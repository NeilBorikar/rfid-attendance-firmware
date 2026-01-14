#include "rfid_reader.h"
#include "config.h"

#include <SPI.h>
#include <MFRC522.h>

// =============================
// INTERNAL
// =============================
static MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
static unsigned long lastScanTime = 0;

// =============================
// INIT
// =============================
void rfid_init() {
    SPI.begin();
    rfid.PCD_Init();
    Serial.println("[RFID] Reader initialized");
}

// =============================
// SCAN CARD
// =============================
bool rfid_scan(String& uid) {
    unsigned long now = millis();

    // Debounce
    if (now - lastScanTime < RFID_DEBOUNCE_DELAY) {
        return false;
    }

    if (!rfid.PICC_IsNewCardPresent()) return false;
    if (!rfid.PICC_ReadCardSerial()) return false;

    uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10) uid += "0";
        uid += String(rfid.uid.uidByte[i], HEX);
    }

    uid.toUpperCase();

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    yield();

    lastScanTime = now;

    Serial.print("[RFID] UID: ");
    Serial.println(uid);

    return true;
}
