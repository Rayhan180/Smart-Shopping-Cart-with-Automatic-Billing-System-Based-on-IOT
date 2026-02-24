# Smart-Shopping-Cart-with-Automatic-Billing-System-Based-on-IOT
Busan IOT Capstone Award

## Overview
Prototype system using Arduino and ESP32 where:

- Arduino reads RFID tags and updates LCD
- Arduino sends UID to ESP32 via UART
- ESP32 connects to Wi-Fi and posts data to server

## Hardware Used
- Arduino UNO/Nano
- ESP32
- MFRC522 RFID reader
- 16x2 I2C LCD

## Communication
- Arduino ↔ ESP32: UART (9600 baud)
- ESP32 ↔ Server: Wi-Fi HTTP POST

## Author
Rayhan Hussain

![IMG_5018](https://github.com/Rayhan180/Smart-Shopping-Cart-with-Automatic-Billing-System-Based-on-IOT/assets/136218406/957bfd96-0073-44b4-9a30-c0d1a5cef7cc)
