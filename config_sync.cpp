#include "config_sync.h"
#include "config.h"
#include "wifi_manager.h"

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// =============================
// INTERNAL STATE
// =============================
static unsigned long lastSyncTime = 0;
static bool deviceEnabled = true;
static int configVersion = -1;

static String currentSSID = "";
static String currentPassword = "";

// =============================
// INIT
// =============================
void config_sync_init() {
    lastSyncTime = 0;
    deviceEnabled = true;
    configVersion = -1;
}

// =============================
// FETCH + APPLY CONFIG
// =============================
static bool fetch_and_apply_config(bool force) {
    unsigned long now = millis();

    if (!force && (now - lastSyncTime < CONFIG_SYNC_INTERVAL)) {
        return false;
    }

    lastSyncTime = now;

    if (WiFi.status() != WL_CONNECTED) {
        yield();
        return false;
    }

    String url = String(BACKEND_BASE_URL) +
                 DEVICE_CONFIG_ENDPOINT + "/" + DEVICE_ID;

    WiFiClientSecure client;
    client.setInsecure();  // Railway TLS

    HTTPClient http;
    http.begin(client, url);
    http.setTimeout(HTTP_TIMEOUT);

    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Device-Key", DEVICE_API_KEY);

    int httpCode = http.GET();
    yield();

    if (httpCode != 200) {
        http.end();
        return false;
    }

    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, http.getString());
    yield();
    http.end();

    if (err) return false;

    int newVersion = doc["version"] | -1;
    if (!force && newVersion == configVersion) return true;

    configVersion = newVersion;

    deviceEnabled = doc["enabled"] | true;

    const char* newSSID = doc["wifi_ssid"] | "";
    const char* newPassword = doc["wifi_password"] | "";

    if (strlen(newSSID) > 0 && currentSSID != newSSID) {
        currentSSID = newSSID;
        currentPassword = newPassword;
        connectWiFi(currentSSID.c_str(), currentPassword.c_str());
    }

    Serial.println("[CONFIG] Synced");
    yield();
    return true;
}

// =============================
// LOOP
// =============================
void config_sync_loop() {
    fetch_and_apply_config(false);
}

// =============================
// FORCE SYNC
// =============================
void force_config_sync() {
    fetch_and_apply_config(true);
}

// =============================
// ACCESSOR
// =============================
bool is_device_enabled() {
    return deviceEnabled;
}
