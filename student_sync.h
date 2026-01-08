#ifndef STUDENT_SYNC_H
#define STUDENT_SYNC_H

#include <Arduino.h>

// =============================
// LIFECYCLE
// =============================
void student_sync_init();
void student_sync_loop();
void force_student_sync();

// =============================
// AUTH CHECK
// =============================
bool is_uid_authorized(const String& uid);

#endif // STUDENT_SYNC_H
