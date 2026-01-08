#ifndef RFID_READER_H
#define RFID_READER_H

#include <Arduino.h>

// =============================
// LIFECYCLE
// =============================
void rfid_init();

// =============================
// SCAN
// =============================
// Returns true if a card is read
// UID is returned as HEX string
bool rfid_scan(String& uid);

#endif // RFID_READER_H
