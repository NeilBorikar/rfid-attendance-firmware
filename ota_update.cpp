#include "ota_update.h"
#include "config.h"

#include <ArduinoOTA.h>

void ota_init() {
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);

    ArduinoOTA.onStart([]() {
        Serial.println("[OTA] Update started");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("[OTA] Update finished");
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.print("[OTA] Error: ");
        Serial.println(error);
    });

    ArduinoOTA.begin();
    Serial.println("[OTA] Ready");
}

void ota_loop() {
    ArduinoOTA.handle();
    yield();

}
