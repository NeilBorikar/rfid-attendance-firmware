#ifndef OFFLINE_QUEUE_H
#define OFFLINE_QUEUE_H

#include <Arduino.h>

// =============================
// LIFECYCLE
// =============================
void offline_queue_init();

// =============================
// QUEUE OPERATIONS
// =============================
void queue_event(const String& uid, const String& timestamp);
void retry_queue();

#endif // OFFLINE_QUEUE_H
