#ifndef CONFIG_H
#define CONFIG_H

// =========================
// DEVICE IDENTITY
// =========================
#define DEVICE_ID "SCHOOL_GATE_1"

// =========================
// BACKEND CONFIG
// =========================
#define BACKEND_BASE_URL "https://api.yourdomain.com"
#define DEVICE_CONFIG_ENDPOINT "/devices/config"
#define ATTENDANCE_EVENT_ENDPOINT "/attendance/event"

// =========================
// TIMING CONFIG (ms)
// =========================
#define WIFI_RETRY_DELAY 5000
#define CONFIG_SYNC_INTERVAL 60000   // 1 minute
#define RFID_DEBOUNCE_DELAY 3000

#endif
