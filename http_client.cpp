#include "http_client.h"
#include "config.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

bool postAttendanceEvent(JsonDocument& payload) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    String url = String(BACKEND_BASE_URL) + ATTENDANCE_EVENT_ENDPOINT;

    https.begin(client, url);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Device-ID", DEVICE_ID);

    String body;
    serializeJson(payload, body);

    int code = https.POST(body);
    https.end();

    return code == 200;
}

bool fetchDeviceConfig(JsonDocument& response) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    String url = String(BACKEND_BASE_URL) + DEVICE_CONFIG_ENDPOINT;

    https.begin(client, url);
    https.addHeader("Device-ID", DEVICE_ID);

    int code = https.GET();
    if (code != 200) {
        https.end();
        return false;
    }

    deserializeJson(response, https.getString());
    https.end();
    return true;
}
