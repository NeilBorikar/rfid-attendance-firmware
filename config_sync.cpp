#include "config_sync.h"
#include "http_client.h"
#include "wifi_manager.h"
#include <ArduinoJson.h>

static String currentSSID = "";
static String currentPassword = "";

void syncDeviceConfig() {
    StaticJsonDocument<256> config;

    if (!fetchDeviceConfig(config)) return;

    const char* newSSID = config["wifi_ssid"];
    const char* newPass = config["wifi_password"];

    if (currentSSID != newSSID) {
        currentSSID = newSSID;
        currentPassword = newPass;
        connectWiFi(newSSID, newPass);
    }
}
