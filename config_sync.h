#ifndef CONFIG_SYNC_H
#define CONFIG_SYNC_H

#include <Arduino.h>

// =============================
// LIFECYCLE
// =============================
void config_sync_init();
void config_sync_loop();
void force_config_sync();

// =============================
// RUNTIME STATE ACCESS
// =============================
bool is_device_enabled();

#endif // CONFIG_SYNC_H
