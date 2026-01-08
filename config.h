#ifndef CONFIG_H
#define CONFIG_H

// ==================================================
// DEVICE IDENTITY
// ==================================================
#define DEVICE_ID "SCHOOL_GATE_1"
#define DEVICE_TYPE "RFID_GATE"
#define FIRMWARE_VERSION "1.1.0"


// ==================================================
// BACKEND BASE
// ==================================================
#define BACKEND_BASE_URL "https://api.yourdomain.com"


// ==================================================
// BACKEND ENDPOINTS (SINGLE SOURCE OF TRUTH)
// ==================================================
#define DEVICE_CONFIG_ENDPOINT     "/devices/config"
#define STUDENT_SYNC_ENDPOINT      "/students"
#define ATTENDANCE_EVENT_ENDPOINT  "/attendance/event"
#define FIRMWARE_UPDATE_ENDPOINT   "/devices/firmware"


// ==================================================
// AUTH / SECURITY (RECOMMENDED)
// ==================================================
#define DEVICE_API_KEY "REPLACE_WITH_SECURE_KEY"


// ==================================================
// WIFI & NETWORK
// ==================================================
#define WIFI_RETRY_DELAY 5000          // ms
#define HTTP_TIMEOUT 8000              // ms


// ==================================================
// SYNC & POLLING INTERVALS
// ==================================================
#define CONFIG_SYNC_INTERVAL   60000   // 1 minute
#define STUDENT_SYNC_INTERVAL  10000   // 10 seconds (near-live)
#define OFFLINE_RETRY_INTERVAL 15000   // 15 seconds


// ==================================================
// RFID SETTINGS
// ==================================================
#define RFID_DEBOUNCE_DELAY 3000       // ms
#define RFID_SS_PIN  5
#define RFID_RST_PIN 27


// ==================================================
// TIME / NTP CONFIG
// ==================================================
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 19800           // IST (UTC +5:30)
#define DAYLIGHT_OFFSET_SEC 0


// ==================================================
// OTA CONFIG
// ==================================================
#define OTA_HOSTNAME "rfid-device"
#define OTA_PASSWORD "rfid_secure_ota"


// ==================================================
// FILE SYSTEM (SPIFFS)
// ==================================================
#define OFFLINE_QUEUE_FILE "/offline_queue.txt"


#endif // CONFIG_H
