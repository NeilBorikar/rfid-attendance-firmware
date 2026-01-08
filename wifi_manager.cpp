#include "wifi_manager.h"

void connectWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}
