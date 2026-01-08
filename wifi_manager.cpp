#include "wifi_manager.h"
#include "config.h"

#include <ESP8266WiFi.h>

// =============================
// INTERNAL STATE
// =============================
static unsigned long lastAttemptTime = 0;
static bool connecting = false;

static String currentSSID = "";
static String currentPassword = "";

// =============================
// INIT
// =============================
void wifi_init() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);

    lastAttemptTime = 0;
    connecting = false;
}

// =============================
// CONNECT (SAFE, NON-BLOCKING)
// =============================
void connectWiFi(const char* ssid, const char* password) {
    if (!ssid || strlen(ssid) == 0) {
        return;
    }

    currentSSID = ssid;
    currentPassword = password;

    Serial.print("[WIFI] Connecting to ");
    Serial.println(currentSSID);

    WiFi.disconnect(true);
    delay(100);

    WiFi.begin(currentSSID.c_str(), currentPassword.c_str());

    connecting = true;
    lastAttemptTime = millis();
}

// =============================
// LOOP (AUTO-RECONNECT)
// =============================
void wifi_loop() {
    if (WiFi.status() == WL_CONNECTED) {
        connecting = false;
        return;
    }

    unsigned long now = millis();

    if (!connecting || (now - lastAttemptTime >= WIFI_RETRY_DELAY)) {
        if (currentSSID.length() > 0) {
            Serial.println("[WIFI] Reconnecting...");
            WiFi.disconnect();
            WiFi.begin(currentSSID.c_str(), currentPassword.c_str());
            connecting = true;
            lastAttemptTime = now;
        }
    }
}

// =============================
// STATUS CHECK
// =============================
bool wifi_is_connected() {
    return WiFi.status() == WL_CONNECTED;
}
