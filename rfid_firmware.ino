#include <Arduino.h>

#include "config.h"
#include "wifi_manager.h"
#include "config_sync.h"
#include "student_sync.h"
#include "rfid_reader.h"
#include "http_client.h"
#include "offline_queue.h"
#include "time_utils.h"
#include "ota_update.h"

String uid;

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("\n[BOOT] RFID Firmware Starting");

    wifi_init();
    time_init();
    offline_queue_init();
    rfid_init();

    config_sync_init();
    student_sync_init();

    ota_init();

    Serial.println("[BOOT] System ready");
}

void loop() {
    wifi_loop();
    ota_loop();

    config_sync_loop();
    student_sync_loop();

    retry_queue();

    if (!is_device_enabled()) {
        delay(500);
        return;
    }

    if (rfid_scan(uid)) {
        if (!is_uid_authorized(uid)) {
            Serial.println("[ACCESS] Unauthorized card");
            return;
        }

        String timestamp = get_iso_timestamp();

        if (!send_attendance(uid, timestamp)) {
            queue_event(uid, timestamp);
        }
    }
}
