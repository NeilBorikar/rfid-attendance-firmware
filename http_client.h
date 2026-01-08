#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <Arduino.h>

// =============================
// ATTENDANCE EVENT
// =============================
bool send_attendance(const String& uid, const String& timestamp);

#endif // HTTP_CLIENT_H
