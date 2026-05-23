🚨 IoT Based Accident Detection and Alert System

An IoT-based smart safety system designed to detect vehicle accidents using an MPU6050 sensor and automatically send emergency alerts with live GPS location through Telegram.

---

📌 Project Overview

The IoT Based Accident Detection and Alert System is an ESP8266-based safety project developed to reduce emergency response time during road accidents. The MPU6050 sensor continuously monitors sudden shocks or impacts. If an accident is detected, the system activates a buzzer and sends an emergency alert message with the live GPS location to Telegram using WiFi connectivity.

---

✨ Features

- Accident detection using MPU6050 sensor
- Real-time GPS location tracking
- Automatic Telegram alert system
- Emergency buzzer indication
- Manual alert cancel button
- WiFi-enabled IoT monitoring system
- Fast emergency notification

---

🛠 Components Used

- ESP8266 NodeMCU
- MPU6050 Accelerometer Sensor
- GPS Module
- Buzzer
- Push Button
- Jumper Wires
- Power Supply

---

⚙ Technologies Used

- Embedded C
- Arduino IDE
- ESP8266 WiFi
- IoT
- Telegram Bot API

---

🔌 Circuit Connections

- MPU6050 SDA → D2
- MPU6050 SCL → D1
- GPS TX → D5
- GPS RX → D6
- Buzzer → D7
- Push Button → D0

---

🚀 Working Principle

The MPU6050 sensor continuously monitors vehicle movement and detects sudden impacts or vibrations. If the shock value exceeds the predefined threshold, the system assumes an accident has occurred and activates the buzzer. A short delay is provided to manually cancel the alert using a push button. If not cancelled, the ESP8266 fetches the live GPS coordinates and sends an emergency alert message with the Google Maps location link to Telegram through WiFi.
