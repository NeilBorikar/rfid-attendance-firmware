#include "time_utils.h"
#include "config.h"

#include <time.h>

// =============================
// INIT NTP
// =============================
void time_init() {
    configTime(
        GMT_OFFSET_SEC,
        DAYLIGHT_OFFSET_SEC,
        NTP_SERVER
    );
    Serial.println("[TIME] NTP initialized");
}

// =============================
// ISO 8601 TIMESTAMP
// =============================
String get_iso_timestamp() {
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) {
        return "1970-01-01T00:00:00";
    }

    char buffer[25];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &timeinfo);
    return String(buffer);
}

// =============================
// UNIX TIMESTAMP
// =============================
unsigned long get_unix_timestamp() {
    time_t now;
    time(&now);
    return (unsigned long)now;
}
