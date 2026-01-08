/****************************************************
 *  GOD-LEVEL RFID ATTENDANCE FIRMWARE (ESP32)
 *  Backend: FastAPI (Railway)
 *  Author: Neil
 ****************************************************/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <time.h>

/* =========================
   CONFIGURATION
   ========================= */

// 🔹 WiFi
#define WIFI_SSID       "YOUR_WIFI_NAME"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

// 🔹 Backend
#define BACKEND_URL     "https://your-backend.up.railway.app/attendance"
#define DEVICE_ID       "SCHOOL_GATE_01"

// 🔹 RFID Pins (ESP32)
#define SS_PIN   5
#define RST_PIN  27

// 🔹 Retry Config
#define MAX_RETRIES     3
#define RETRY_DELAY_MS  2000

/* =========================
   GLOBAL OBJECTS
   ========================= */

MFRC522 rfid(SS_PIN, RST_PIN);
WiFiClientSecure secureClient;

/* =========================
   WIFI MANAGEMENT
   ========================= */

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
}

/* =========================
   TIME SYNC (IMPORTANT)
   ========================= */

void syncTime() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Syncing time");

  time_t now;
  while ((now = time(nullptr)) < 100000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTime synced");
}

/* =========================
   UID UTIL
   ========================= */

String getCardUID() {
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

/* =========================
   API CLIENT
   ========================= */

bool sendAttendance(const String& uid) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return false;
  }

  HTTPClient http;
  secureClient.setInsecure(); // Railway uses valid TLS, but ESP cert store is limited

  http.begin(secureClient, BACKEND_URL);
  http.addHeader("Content-Type", "application/json");

  // JSON Payload
  StaticJsonDocument<256> doc;
  doc["uid"] = uid;
  doc["device_id"] = DEVICE_ID;
  doc["event_type"] = "IN";
  doc["timestamp"] = time(nullptr);

  String payload;
  serializeJson(doc, payload);

  int httpCode = http.POST(payload);
  http.end();

  if (httpCode == 201 || httpCode == 200) {
    Serial.println("Attendance sent successfully");
    return true;
  } else {
    Serial.printf("Server error: %d\n", httpCode);
    return false;
  }
}

/* =========================
   RFID LOOP HANDLER
   ========================= */

void handleRFID() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String uid = getCardUID();
  Serial.print("Card detected: ");
  Serial.println(uid);

  bool success = false;

  for (int i = 1; i <= MAX_RETRIES; i++) {
    Serial.printf("Attempt %d...\n", i);
    if (sendAttendance(uid)) {
      success = true;
      break;
    }
    delay(RETRY_DELAY_MS);
  }

  if (!success) {
    Serial.println("❌ Attendance failed (will retry next scan)");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

/* =========================
   SETUP
   ========================= */

void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI.begin();
  rfid.PCD_Init();

  connectWiFi();
  syncTime();

  Serial.println("RFID Attendance System Ready");
}

/* =========================
   LOOP
   ========================= */

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  handleRFID();
}
