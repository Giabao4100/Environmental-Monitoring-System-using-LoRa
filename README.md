System Description – Environmental Monitoring System using LoRa 
This project presents a low-power IoT environmental monitoring system based on a LoRa Gateway–Node architecture. Each sensor node, built with an STM32F103C8T6, measures temperature, humidity, and gas concentration using DHT11 and MQ-2 sensors, then transmits the data wirelessly to an ESP32-based Gateway via LoRa. The Gateway forwards the collected data to the ERa IoT platform over Wi-Fi, enabling real-time monitoring, historical data visualization, and remote alerts. The system also provides local OLED display and buzzer/LED alarms when gas concentration exceeds predefined thresholds. Designed for long-range communication, low power consumption, and easy deployment, it is suitable for smart agriculture, warehouses, factories, and smart building applications.

Gateway operation diagram
<img width="490" height="403" alt="GW" src="https://github.com/user-attachments/assets/dccc76f0-419d-4fdc-b8e6-63bbf8b58eda" />

Activity diagram at Node
<img width="664" height="401" alt="Node" src="https://github.com/user-attachments/assets/b6f7ab44-92f5-4de0-95ce-f23b2e2a9218" />

