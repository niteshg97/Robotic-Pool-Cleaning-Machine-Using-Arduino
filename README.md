<img width="356" height="312" alt="COMPONENTS-OF-THE-AGRICULTURAL-ROBOT" src="https://github.com/user-attachments/assets/6aecd877-5c9c-4c72-ace9-86d8f522f2cb" />
# 🤖 Agribot — Autonomous Agriculture Automation Robot

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
![Platform](https://img.shields.io/badge/platform-Arduino%20%7C%20Raspberry%20Pi-green)
![IoT](https://img.shields.io/badge/IoT-MQTT-orange)

---

## 📌 Overview
**Agribot** is an autonomous, multi-functional agricultural robot developed to **automate essential farming tasks** such as:
- **Soil moisture sensing**
- **Seed dispensing**
- **Humidity monitoring**
- **Real-time data logging** using MQTT over Wi-Fi

This system is designed to assist farmers in **precision agriculture** by automating repetitive processes and providing live environmental data for decision-making.

> 🏆 **Awarded 2nd Prize** among 100+ teams at **NIT Patna Tech Fest** for AgriTech innovation.

---

## 🛠 Features
- **Autonomous Navigation** — Operates without manual control for predefined field coverage.
- **Soil Moisture Sensing** — Detects dryness level and decides whether irrigation is required.
- **Seed Dispensing Mechanism** — Dispenses seeds at fixed intervals or based on soil readiness.
- **Humidity Monitoring** — Tracks environmental conditions for crop health analysis.
- **IoT Integration (MQTT)** — Sends live data to a central dashboard for logging and analytics.

---

## ⚙️ Hardware Components
| Component                  | Purpose |
|---------------------------|---------|
| **Arduino Uno**           | Controls soil moisture sensor, seed dispensing motor, and humidity sensor |
| **Raspberry Pi 4**        | Acts as IoT gateway, processes data, and manages MQTT communication |
| **Soil Moisture Sensor**  | Measures moisture content in soil |
| **DHT11/DHT22 Sensor**    | Monitors ambient temperature and humidity |
| **DC Motors + Motor Driver** | Drives the robot autonomously |
| **Servo Motor**           | Controls seed dispensing mechanism |
| **Battery Pack**          | Powers both Arduino and Raspberry Pi modules |

---

## 🖥 Software & Communication
- **Arduino IDE** — Microcontroller programming for sensor data collection and actuator control.
- **Python** — Data processing and MQTT client implementation on Raspberry Pi.
- **MQTT Protocol** — Lightweight messaging protocol for real-time data transmission.
- **Broker** — Mosquitto (local or cloud-hosted).

---

## 📊 System Workflow
```mermaid
flowchart LR
    A[Soil Moisture Sensor] --> D[Arduino]
    B[Humidity Sensor] --> D
    D --> E[Motor & Seed Mechanism Control]
    D --> F[Raspberry Pi]
    F --> G[MQTT Broker]
    G --> H[Cloud Dashboard / Local UI]

