#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

// =============================
// LIFECYCLE
// =============================
void wifi_init();
void wifi_loop();

// =============================
// CONNECTION CONTROL
// =============================
bool wifi_is_connected();
void connectWiFi(const char* ssid, const char* password);

#endif // WIFI_MANAGER_H
