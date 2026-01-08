#include "config_sync.h"
#include "config.h"
#include "wifi_manager.h"

#include <HTTPClient.h>
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
        return false;
    }

    HTTPClient http;
    String url = String(BACKEND_BASE_URL) +
                 DEVICE_CONFIG_ENDPOINT + "/" + DEVICE_ID;

    http.begin(url);
    http.setTimeout(HTTP_TIMEOUT);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Device-Key", DEVICE_API_KEY);

    int httpCode = http.GET();
    if (httpCode != 200) {
        http.end();
        return false;
    }

    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, http.getString());
    http.end();

    if (err) {
        return false;
    }

    int newVersion = doc["version"] | -1;
    if (!force && newVersion == configVersion) {
        return true;  // No change
    }

    configVersion = newVersion;

    // =============================
    // APPLY DEVICE FLAGS
    // =============================
    deviceEnabled = doc["enabled"] | true;

    // =============================
    // APPLY WIFI CONFIG (HOT UPDATE)
    // =============================
    const char* newSSID = doc["wifi_ssid"] | "";
    const char* newPassword = doc["wifi_password"] | "";

    if (strlen(newSSID) > 0 && currentSSID != newSSID) {
        currentSSID = newSSID;
        currentPassword = newPassword;

        Serial.println("WiFi config updated from backend");
        connectWiFi(currentSSID.c_str(), currentPassword.c_str());
    }

    Serial.println("Device config synced");
    Serial.print("Enabled: ");
    Serial.println(deviceEnabled);

    return true;
}

// =============================
// LOOP (SAFE POLLING)
// =============================
void config_sync_loop() {
    fetch_and_apply_config(false);
}

// =============================
// MANUAL / FORCE SYNC
// =============================
void force_config_sync() {
    fetch_and_apply_config(true);
}

// =============================
// RUNTIME ACCESS
// =============================
bool is_device_enabled() {
    return deviceEnabled;
}
