#define ERA_DEBUG
#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"
#define ERA_AUTH_TOKEN "3bbcf27e-50a9-4f45-ab2b-9a029bfd884e"

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ERa.hpp>

// ================= WIFI =================
const char ssid[] = "GBG";
const char pass[] = "duyhaibaothanh12345";

WiFiClient mbTcpClient; 

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ================= LORA UART =================
HardwareSerial mySerial(1);

#define LORA_RX 16
#define LORA_TX 17

char recvBuffer[128];
int idx = 0;

// ================= SENSOR =================
float temp = 0;
float hum = 0;
float ppm = 0;
int tt = 0;

// ================= TIMESTAMP =================
String getTimestamp() {
    unsigned long ms = millis();
    unsigned long totalSec = ms / 1000;
    unsigned long hours   = (totalSec / 3600) % 24;
    unsigned long minutes = (totalSec % 3600) / 60;
    unsigned long seconds = totalSec % 60;
    unsigned long milsec  = ms % 1000;

    char buf[20];
    sprintf(buf, "%02lu:%02lu:%02lu.%03lu", hours, minutes, seconds, milsec);
    return String(buf);
}

// ================= ERa CALLBACKS =================
ERA_CONNECTED() {
    ERA_LOG("ERa", "ERa connected!");
}

ERA_DISCONNECTED() {
    ERA_LOG("ERa", "ERa disconnected!");
}

// ================= OLED =================
void updateOLED() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("ESP32 Gateway");

    display.setCursor(0, 20);
    display.print("Temp: ");
    display.print(temp, 1);      // KHONG xuong dong
       // ky tu ° in ngay sau so
    display.println(" C");        // "C" + xuong dong

    display.setCursor(0, 30);
    display.print("Hum : ");
    display.print(hum, 1);       // KHONG xuong dong
    display.println(" %");       // " %" + xuong dong

    display.setCursor(0, 40);
    display.print("PPM : ");
    display.println(ppm, 0);

    display.setCursor(0, 50);    // doi sang dong moi, KHONG con trung voi Hum
    if (tt == 2) {
        display.println("NGUY HIEM");
    }
    else if (tt == 1) {
        display.println("CANH BAO");
    }
    else {
        display.println("AN TOAN");
    }

    display.display();
}

// ================= ERa =================
void timerEvent() {
    if (WiFi.status() != WL_CONNECTED)
        return;

    ERa.virtualWrite(V0, temp);
    ERa.virtualWrite(V1, hum);
    ERa.virtualWrite(V2, ppm);

    if (tt == 2)
    
        ERa.virtualWrite(V3, "NGUY HIEM");
      
    
    else if (tt == 1)
        ERa.virtualWrite(V3, "CANH BAO");
      
    
    else
        ERa.virtualWrite(V3, "AN TOAN");
      
    
}

// ================= PARSE =================
// ================= PARSE =================
void parseUART1Data(char* data) {

    // Nếu là log từ STM32
    if (strncmp(data, "LOG:", 4) == 0) {
        Serial.print(getTimestamp());
        Serial.print(" -> ");
        Serial.println(data + 4);
        return;
    }

    // Parse sensor data bình thường
    if (sscanf(data,
               "temp:%f;hum:%f;ppm:%f;tt:%d",
               &temp, &hum, &ppm, &tt) == 4) {

        Serial.print(getTimestamp());
        Serial.printf(" -> Temp=%.1f Hum=%.1f PPM=%.1f TT=%d\n",
                      temp, hum, ppm, tt);

        updateOLED();
    }
    else {
        Serial.print(getTimestamp());
        Serial.print(" -> Sai format: ");
        Serial.println(data);

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);

        display.setCursor(0, 0);
        display.println("Sai format chuoi");

        display.setCursor(0, 16);
        display.println(data);

        display.display();
    }
}

// ================= RECEIVE =================
void receive_UART1_data() {
    while (mySerial.available()) {
        char c = mySerial.read();

        if (c == '\n' || c == '\r') {
            if (idx > 0) {
                recvBuffer[idx] = '\0';

                if (strncmp(recvBuffer, "LOG:", 4) != 0) {
                    Serial.print(getTimestamp());
                    Serial.print(" -> Received LoRa data: ");
                    Serial.println(recvBuffer);
                }

                parseUART1Data(recvBuffer);
                idx = 0;
            }
        }
        else {
            if (idx < (int)sizeof(recvBuffer) - 1) {
                recvBuffer[idx++] = c;
            }
            else {
                // Buffer đầy → reset và bỏ qua chuỗi lỗi
                Serial.print(getTimestamp());
                Serial.println(" -> Buffer overflow, reset!");
                idx = 0;
            }
        }
    }
}

// ================= SETUP =================
void setup() {

#if defined(ERA_DEBUG)
    Serial.begin(115200);
#endif

    mySerial.setRxBufferSize(1024);
    mySerial.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX);

    Wire.begin(21, 22);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        while (true) {
            delay(1000);
        }
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("Dang khoi dong...");
    display.display();

    ERa.setModbusClient(mbTcpClient);
    ERa.setScanWiFi(true);

    ERa.begin(ssid, pass);

    ERa.addInterval(1500L, timerEvent);

    display.clearDisplay();

    display.setCursor(0, 0);
    display.println("Gateway Ready");

    display.setCursor(0, 16);
    display.println("Cho du lieu LoRa...");

    display.display();
}

// ================= LOOP =================
void loop() {
    ERa.run();
    receive_UART1_data();
}
