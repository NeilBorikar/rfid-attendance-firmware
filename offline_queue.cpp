#include "offline_queue.h"
#include "config.h"
#include "http_client.h"

#include <SPIFFS.h>

// =============================
// INIT
// =============================
void offline_queue_init() {
    if (!SPIFFS.begin(true)) {
        Serial.println("[OFFLINE_QUEUE] SPIFFS mount failed");
    } else {
        Serial.println("[OFFLINE_QUEUE] SPIFFS mounted");
    }
}

// =============================
// ADD EVENT TO QUEUE
// =============================
void queue_event(const String& uid, const String& timestamp) {
    File file = SPIFFS.open(OFFLINE_QUEUE_FILE, FILE_APPEND);
    if (!file) {
        Serial.println("[OFFLINE_QUEUE] Failed to open queue file");
        return;
    }

    file.print(uid);
    file.print("|");
    file.println(timestamp);

    file.close();
    Serial.println("[OFFLINE_QUEUE] Event queued");
}

// =============================
// RETRY QUEUED EVENTS
// =============================
void retry_queue() {
    if (!SPIFFS.exists(OFFLINE_QUEUE_FILE)) {
        return;
    }

    File file = SPIFFS.open(OFFLINE_QUEUE_FILE, FILE_READ);
    if (!file) {
        return;
    }

    String remainingData = "";

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() == 0) continue;

        int sep = line.indexOf('|');
        if (sep == -1) {
            // corrupted line → skip
            continue;
        }

        String uid = line.substring(0, sep);
        String timestamp = line.substring(sep + 1);

        bool sent = send_attendance(uid, timestamp);
        if (!sent) {
            remainingData += line + "\n";
        }
    }

    file.close();

    // Rewrite queue with remaining failed events
    SPIFFS.remove(OFFLINE_QUEUE_FILE);

    if (remainingData.length() > 0) {
        File rewrite = SPIFFS.open(OFFLINE_QUEUE_FILE, FILE_WRITE);
        if (rewrite) {
            rewrite.print(remainingData);
            rewrite.close();
        }
    }
}
