# RaspberryPi/mqtt_bridge.py
import serial
import time
import threading
import argparse
import json
from paho.mqtt import client as mqtt

#Configuration
SERIAL_PORT = "/dev/ttyACM0"   
BAUDRATE = 115200

MQTT_BROKER = "localhost"  
MQTT_PORT = 1883
MQTT_TOPIC_SOIL = "agri/soil"
MQTT_TOPIC_ENV = "agri/env"
MQTT_TOPIC_EVENTS = "agri/events"
MQTT_TOPIC_ACK = "agri/acks"
MQTT_CMD_TOPIC = "agri/cmd/#"  

# === Serial helper ===
ser = None

def open_serial():
    global ser
    while True:
        try:
            ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)
            print("Serial opened:", SERIAL_PORT)
            break
        except Exception as e:
            print("Waiting for serial device...", e)
            time.sleep(2)

# === MQTT client ===
client = mqtt.Client()

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker with rc:", rc)
    client.subscribe(MQTT_CMD_TOPIC)
    print("Subscribed to:", MQTT_CMD_TOPIC)

def on_message(client, userdata, msg):
    # Incoming command from MQTT; forward to Arduino via serial
    try:
        topic = msg.topic
        payload = msg.payload.decode('utf-8')
        print(f"MQTT CMD -> topic: {topic} payload: {payload}")
        # Expect payload to be simple command string, e.g. "CMD,SEED,1" or "MODE,MANUAL"
        send_serial_command(payload)
    except Exception as e:
        print("Error handling MQTT message:", e)

def send_serial_command(command_str):
    if ser and ser.is_open:
        if not command_str.endswith("\n"):
            command_str += "\n"
        ser.write(command_str.encode('utf-8'))
        print("Forwarded to Arduino:", command_str.strip())
    else:
        print("Serial not open; cannot send command")

# === Serial reader thread ===
def serial_reader_thread():
    while True:
        try:
            line = ser.readline().decode('utf-8').strip()
            if not line:
                continue
            print("SERIAL:", line)
            # Parse messages like:
            # SENS,soil_raw,soil_pct,tempC,humidity
            # EVENT,SOIL_DRY
            # ACK,SEED,OK
            parts = line.split(",")
            if parts[0] == "SENS":
                # parse sensor values
                if len(parts) >= 5:
                    soil_raw = int(parts[1])
                    soil_pct = int(parts[2])
                    tempC = float(parts[3])
                    humid = float(parts[4])
                    # Publish JSON
                    client.publish(MQTT_TOPIC_SOIL, json.dumps({
                        "soil_raw": soil_raw,
                        "soil_pct": soil_pct,
                        "timestamp": int(time.time())
                    }))
                    client.publish(MQTT_TOPIC_ENV, json.dumps({
                        "tempC": tempC,
                        "humidity": humid,
                        "timestamp": int(time.time())
                    }))
            elif parts[0] == "EVENT":
                client.publish(MQTT_TOPIC_EVENTS, json.dumps({
                    "event": parts[1] if len(parts) > 1 else "UNKNOWN",
                    "raw": line, "timestamp": int(time.time())
                }))
            elif parts[0] == "ACK":
                client.publish(MQTT_TOPIC_ACK, json.dumps({"ack": parts[1:], "raw": line}))
            else:
                # publish unknown raw line to events for debugging
                client.publish(MQTT_TOPIC_EVENTS, json.dumps({"raw": line}))
        except Exception as e:
            print("Serial read error:", e)
            time.sleep(0.5)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--serial", default=SERIAL_PORT)
    parser.add_argument("--broker", default=MQTT_BROKER)
    args = parser.parse_args()
    global ser, client
    SERIAL_PORT_LOCAL = args.serial
    MQTT_BROKER_LOCAL = args.broker

    # open serial
    open_serial()

    # Setup MQTT
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(MQTT_BROKER_LOCAL, MQTT_PORT, keepalive=60)
    client.loop_start()

    # Start serial reader thread
    t = threading.Thread(target=serial_reader_thread, daemon=True)
    t.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        client.loop_stop()
        if ser and ser.is_open:
            ser.close()

if __name__ == "__main__":
    main()
