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


## 📸 Project Presentation
The **Agribot** was showcased at the **NIT Patna Tech Fest**, where it demonstrated its capabilities in **soil moisture sensing, seed dispensing, humidity monitoring**, and **MQTT-based IoT data logging**.  
Our live demonstration highlighted how **autonomous agricultural robotics** can improve efficiency and precision in farming operations.

<p align="center">
  <img src="images/project_presentation.jpg" alt="Presenting Agribot at NIT Patna Tech Fest" width="80%">
</p>
<img width="553" height="517" alt="Screenshot 2025-08-08 at 7 36 46 PM" src="https://github.com/user-attachments/assets/2fb50efb-06b8-4511-b7fe-dc26e7508e48" />


## 🏆 Certificate of Achievement
Our team was honored with the **2nd Prize** in the **AgriTech Innovation category** at the **NIT Patna Tech Fest**.  
This award recognized **Agribot’s innovative approach** in integrating autonomous robotics with IoT-based data logging to address real-world agricultural challenges.


<p align="center">
  <img src="images/Certificate.jpg" alt="Presenting Agribot at NIT Patna Tech Fest" width="80%">
<img width="550" height="500" alt="Screenshot 2025-08-08 at 7 36 46 PM" src="https://github.com/user-attachments/assets/ea5640ac-a0c7-47c3-9a74-00e09bd635ca" />
