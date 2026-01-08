#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <Arduino.h>

// =============================
// TIME INIT
// =============================
void time_init();

// =============================
// TIME ACCESSORS
// =============================
String get_iso_timestamp();
unsigned long get_unix_timestamp();

#endif // TIME_UTILS_H
