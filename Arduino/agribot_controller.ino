
// Agribot controller: sensors + actuators + Serial command interface
#include <Arduino.h>
#include <Servo.h>
#include "DHT.h"

// pin
const int PIN_SOIL = A0;          
const int PIN_DHT = 2;            
const int PIN_SEED_SERVO = 9;     
const int PIN_MOTOR_IN1 = 5;      
const int PIN_MOTOR_IN2 = 6;      
const int PIN_MOTOR_EN = 3;       
const int LED_PIN = 13;


#define DHTTYPE DHT22


const int soil_dry = 800;   
const int soil_wet = 300;   


const unsigned long SEND_INTERVAL_MS = 2000; 


DHT dht(PIN_DHT, DHTTYPE);
Servo seedServo;
unsigned long lastSend = 0;
bool autonomousMode = true;


void setMotor(int speed) {
 
  if (speed > 0) {
    analogWrite(PIN_MOTOR_IN1, constrain(speed,0,255));
    analogWrite(PIN_MOTOR_IN2, 0);
  } else if (speed < 0) {
    analogWrite(PIN_MOTOR_IN1, 0);
    analogWrite(PIN_MOTOR_IN2, constrain(-speed,0,255));
  } else {
    analogWrite(PIN_MOTOR_IN1, 0);
    analogWrite(PIN_MOTOR_IN2, 0);
  }
}

// Map analog soil reading to percentage (0% dry — 100% wet)
int soilPercent(int raw) {
  raw = constrain(raw, 0, 1023);
  float pct = 100.0f * (float)(soil_dry - raw) / (float)(soil_dry - soil_wet);
  pct = constrain(pct, 0.0, 100.0);
  return (int)pct;
}

// Dispense seed: rotate servo to dispense (adjust angles & delays)
void dispenseSeed(int cycles = 1) {
  for (int c = 0; c < cycles; ++c) {
    seedServo.write(110);   // move to dispensing pos
    delay(600);
    seedServo.write(40);    // return
    delay(600);
  }
}

// Parse incoming serial command lines
void handleCommand(String cmdline) {
  cmdline.trim();
  if (cmdline.length() == 0) return;

  char buf[200];
  cmdline.toCharArray(buf, sizeof(buf));
  byte idx = 0;
  char *token = strtok(buf, ",");
  String parts[6];
  while (token != NULL && idx < 6) {
    parts[idx++] = String(token);
    token = strtok(NULL, ",");
  }
  if (idx == 0) return;

  if (parts[0] == "CMD") {
    if (parts[1] == "SEED") {
      int cycles = 1;
      if (idx >= 3) cycles = parts[2].toInt();
      dispenseSeed(cycles);
      Serial.println("ACK,SEED,OK");
    } else if (parts[1] == "MOTOR") {
      
      if (idx >= 4 && parts[2] == "SPEED") {
        int val = parts[3].toInt();
        setMotor(constrain(val, -255, 255));
        Serial.println("ACK,MOTOR,OK");
      } else if (idx >= 3) {
        
        String dir = parts[2];
        if (dir == "FORWARD") setMotor(160);
        else if (dir == "REVERSE") setMotor(-160);
        else setMotor(0);
        Serial.println("ACK,MOTOR,OK");
      }
    }
  } else if (parts[0] == "MODE") {
    if (parts[1] == "AUTO") {
      autonomousMode = true;
      Serial.println("ACK,MODE,AUTO");
    } else if (parts[1] == "MANUAL") {
      autonomousMode = false;
      Serial.println("ACK,MODE,MANUAL");
    }
  } else if (parts[0] == "PING") {
    Serial.println("PONG");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);

  // Motor pins
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);
  pinMode(PIN_MOTOR_EN, OUTPUT);

  // initialize servo
  seedServo.attach(PIN_SEED_SERVO);
  seedServo.write(40); // resting position

  // DHT sensor
  dht.begin();

  // initial motor off
  setMotor(0);

  Serial.println("AGRIBOT_START");
}

void loop() {
  // read serial commands (non-blocking)
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    handleCommand(line);
  }

  unsigned long now = millis();
  if (now - lastSend >= SEND_INTERVAL_MS) {
    lastSend = now;

    int soil_raw = analogRead(PIN_SOIL);
    int soil_pct = soilPercent(soil_raw);

    float humidity = dht.readHumidity();
    float tempC = dht.readTemperature();

    // If sensor read failed, DHT returns NAN; handle it
    if (isnan(humidity) || isnan(tempC)) {
      humidity = -1.0;
      tempC = -100.0;
    }

    // Format: SENS,soil_raw,soil_pct,temperatureC,humidity
    String out = "SENS,";
    out += String(soil_raw) + ",";
    out += String(soil_pct) + ",";
    out += String(tempC,1) + ",";
    out += String(humidity,1);
    Serial.println(out);

    // Simple autonomous behavior: if very dry, dispense seed or stop (example)
    if (autonomousMode) {
      if (soil_pct < 20) {
        // soil too dry -> maybe don't dispense, but signal
        Serial.println("EVENT,SOIL_DRY");
        // optionally stop motors
        setMotor(0);
      } else if (soil_pct > 50) {
        // ready -> dispense seed (example)
        // This is only a demo auto-action; set proper logic per field plan
        Serial.println("EVENT,SOIL_READY_DISPENSE");
        dispenseSeed(1);
      }
    }
  }
}

