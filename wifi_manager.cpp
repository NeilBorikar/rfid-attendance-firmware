#include "wifi_manager.h"
#include "config.h"

#include <ESP8266WiFi.h>

// =============================
// INTERNAL STATE
// =============================
static unsigned long lastAttemptTime = 0;
static bool connecting = false;

static String currentSSID = "YOUR_WIFI_NAME";
static String currentPassword = "YOUR_WIFI_PASSWORD";


// =============================
// INIT
// =============================
void wifi_init() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);

    Serial.print("[WIFI] Bootstrapping SSID: ");
    Serial.println(currentSSID);

    WiFi.begin(currentSSID.c_str(), currentPassword.c_str());

    lastAttemptTime = millis();
    connecting = true;
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
    wl_status_t status = WiFi.status();

    if (status == WL_CONNECTED) {
        static bool wasConnected = false;
        if (!wasConnected) {
            Serial.print("[WIFI] Connected, IP: ");
            Serial.println(WiFi.localIP());
            wasConnected = true;
        }
        return;
    }

    static unsigned long lastRetry = 0;
    unsigned long now = millis();

    if (now - lastRetry >= WIFI_RETRY_DELAY) {
        Serial.println("[WIFI] Reconnecting...");
        WiFi.disconnect();
        WiFi.begin(currentSSID.c_str(), currentPassword.c_str());
        lastRetry = now;
    }
}



// =============================
// STATUS CHECK
// =============================
bool wifi_is_connected() {
    return WiFi.status() == WL_CONNECTED;
}
