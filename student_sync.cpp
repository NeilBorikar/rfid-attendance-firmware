#include "student_sync.h"
#include "config.h"
#include "wifi_manager.h"

#include <HTTPClient.h>
#include <ArduinoJson.h>

// =============================
// CONFIG
// =============================
#define MAX_STUDENTS 500   // adjust as per school size

// =============================
// INTERNAL STATE
// =============================
static String studentUIDs[MAX_STUDENTS];
static int studentCount = 0;

static unsigned long lastSyncTime = 0;
static int studentsVersion = -1;

// =============================
// INIT
// =============================
void student_sync_init() {
    studentCount = 0;
    studentsVersion = -1;
    lastSyncTime = 0;
}

// =============================
// FETCH + APPLY STUDENTS
// =============================
static bool fetch_and_apply_students(bool force) {
    unsigned long now = millis();

    if (!force && (now - lastSyncTime < STUDENT_SYNC_INTERVAL)) {
        return false;
    }

    lastSyncTime = now;

    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }

    HTTPClient http;
    String url = String(BACKEND_BASE_URL) +
                 STUDENT_SYNC_ENDPOINT +
                 "?device_id=" + DEVICE_ID;

    http.begin(url);
    http.setTimeout(HTTP_TIMEOUT);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Device-Key", DEVICE_API_KEY);

    int httpCode = http.GET();
    if (httpCode != 200) {
        http.end();
        return false;
    }

    StaticJsonDocument<4096> doc;
    DeserializationError err = deserializeJson(doc, http.getString());
    http.end();

    if (err) {
        return false;
    }

    int newVersion = doc["version"] | -1;
    if (!force && newVersion == studentsVersion) {
        return true; // no change
    }

    studentsVersion = newVersion;

    JsonArray arr = doc["students"].as<JsonArray>();
    studentCount = 0;

    for (JsonObject s : arr) {
        if (studentCount >= MAX_STUDENTS) break;
        studentUIDs[studentCount++] = String((const char*)s["uid"]);
    }

    Serial.print("[STUDENT_SYNC] Students loaded: ");
    Serial.println(studentCount);

    return true;
}

// =============================
// LOOP (POLLING)
// =============================
void student_sync_loop() {
    fetch_and_apply_students(false);
}

// =============================
// FORCE SYNC (MANUAL / FUTURE MQTT)
// =============================
void force_student_sync() {
    fetch_and_apply_students(true);
}

// =============================
// AUTHORIZATION CHECK
// =============================
bool is_uid_authorized(const String& uid) {
    for (int i = 0; i < studentCount; i++) {
        if (studentUIDs[i] == uid) {
            return true;
        }
    }
    return false;
}
