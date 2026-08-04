# 🌱 Environmental Monitoring System using LoRa

## 📖 System Description

This project presents a low-power IoT environmental monitoring system based on a **LoRa Gateway–Node architecture**.

Each sensor node, built with an **STM32F103C8T6**, measures **temperature, humidity, and gas concentration** using **DHT11** and **MQ-2** sensors. The collected data is transmitted wirelessly to an **ESP32-based Gateway** via LoRa. The Gateway then forwards the data to the **ERa IoT platform** over Wi-Fi, enabling **real-time monitoring**, **historical data visualization**, and **remote alerts**.

To enhance local monitoring, the system integrates an **OLED display**, **buzzer**, and **LED indicators** that activate when gas concentration exceeds predefined thresholds.

Designed for **long-range communication**, **low power consumption**, and **easy deployment**, this solution is suitable for **smart agriculture**, **warehouses**, **factories**, and **smart building applications**.

---

## 📡 Gateway Operation Diagram

<p align="center">
  <img src="https://github.com/user-attachments/assets/dccc76f0-419d-4fdc-b8e6-63bbf8b58eda" width="750">
</p>

---

## 🌐 Node Operation Diagram

<p align="center">
  <img src="https://github.com/user-attachments/assets/b6f7ab44-92f5-4de0-95ce-f23b2e2a9218" width="750">
</p>

## 📡 Hardware at Gateway
<p align="center">
<img width="761" height="596" alt="Screenshot 2026-08-04 095045" src="https://github.com/user-attachments/assets/ee12f8e8-d7f6-42ee-aa79-46093d77c03c" />
</p>

## 📡 Hardware at Node
<p align="center">
<img width="544" height="569" alt="Screenshot 2026-08-04 095055" src="https://github.com/user-attachments/assets/1796a514-0701-4360-9b69-c7521c0da95c" />
</p>













