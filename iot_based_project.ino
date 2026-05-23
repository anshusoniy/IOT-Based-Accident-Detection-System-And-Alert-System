#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <MPU6050.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// -------- WiFi --------
const char* ssid = "soni";
const char* pass = "12345678";

// -------- Telegram --------
String token = "8648896477:AAH3b8OvoTZd5f1R39lNhxHAHmvw5tzGLmI";
String chat_id = "5516144751";

// -------- Pins --------
#define BUZZER D7
#define BUTTON D0
#define SDA_PIN D2
#define SCL_PIN D1
#define GPS_RX D5
#define GPS_TX D6

MPU6050 mpu;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

// -------- Settings --------
long threshold = 18000;

// -------- Control --------
bool accidentTriggered = false;
unsigned long lastTriggerTime = 0;

// -------- Send Telegram --------
void sendMessage(String text) {
  WiFiClient client;
  HTTPClient http;

  String url = "https://api.telegram.org/bot" + token +
               "/sendMessage?chat_id=" + chat_id +
               "&text=" + text;

  http.begin(client, url);
  http.GET();
  http.end();
}

// -------- Get GPS --------
String getLocation() {
  unsigned long t = millis();

  while (millis() - t < 8000) {
    while (gpsSerial.available()) {
      gps.encode(gpsSerial.read());
    }

    if (gps.location.isValid()) {
      float lat = gps.location.lat();
      float lon = gps.location.lng();

      return "https://maps.google.com/?q=" +
             String(lat, 6) + "," + String(lon, 6);
    }
  }

  return "Location not available";
}

// -------- Setup --------
void setup() {
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  digitalWrite(BUZZER, LOW);

  Wire.begin(SDA_PIN, SCL_PIN);
  mpu.initialize();

  gpsSerial.begin(9600);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  sendMessage("ESP8266 Connected!");
}

// -------- Loop --------
void loop() {

  // -------- Cooldown Mode --------
  if (accidentTriggered) {

    // Button press = manual reset
    if (digitalRead(BUTTON) == LOW) {
      Serial.println("Manual Reset by Button");

      digitalWrite(BUZZER, LOW);
      accidentTriggered = false;
      delay(500);
      return;
    }

    Serial.println("Cooldown...");
    delay(500);

    if (millis() - lastTriggerTime > 7000) {
      accidentTriggered = false;
      Serial.println("System Ready Again");
    }

    return;
  }

  // -------- Sensor Read --------
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  long shock = (abs(ax) + abs(ay) + abs(az)) / 3;

  if (shock > 50000) shock = 50000;

  Serial.print("Shock: ");
  Serial.println(shock);

  // -------- Accident Detect --------
  if (shock > threshold) {

    Serial.println("Accident Detected!");

    accidentTriggered = true;
    lastTriggerTime = millis();

    digitalWrite(BUZZER, HIGH);

    // -------- Cancel Window --------
    unsigned long t = millis();
    bool cancel = false;

    while (millis() - t < 10000) {

      if (digitalRead(BUTTON) == LOW) {
        Serial.println("Alert Cancelled by Button");

        cancel = true;
        digitalWrite(BUZZER, LOW);
        delay(200); // debounce
        break;
      }

      delay(100);
    }

    // -------- Send Alert --------
    if (!cancel) {
      digitalWrite(BUZZER, LOW);

      String location = getLocation();
      sendMessage("🚨 Accident Detected!\n" + location);
    }
  }

  delay(500);
}