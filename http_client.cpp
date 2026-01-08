#include "http_client.h"
#include "config.h"
#include "wifi_manager.h"

#include <HTTPClient.h>
#include <ArduinoJson.h>

// =============================
// SEND ATTENDANCE EVENT
// =============================
bool send_attendance(const String& uid, const String& timestamp) {
    if (!wifi_is_connected()) {
        return false;
    }

    HTTPClient http;
    String url = String(BACKEND_BASE_URL) + ATTENDANCE_EVENT_ENDPOINT;

    http.begin(url);
    http.setTimeout(HTTP_TIMEOUT);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Device-Key", DEVICE_API_KEY);

    StaticJsonDocument<256> doc;
    doc["device_id"] = DEVICE_ID;
    doc["uid"] = uid;
    doc["timestamp"] = timestamp;

    String payload;
    serializeJson(doc, payload);

    int httpCode = http.POST(payload);
    http.end();

    if (httpCode == 200 || httpCode == 201) {
        Serial.println("[HTTP] Attendance sent");
        return true;
    }

    Serial.print("[HTTP] Attendance failed: ");
    Serial.println(httpCode);
    return false;
}
