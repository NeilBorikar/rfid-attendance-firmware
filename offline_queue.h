#ifndef OFFLINE_QUEUE_H
#define OFFLINE_QUEUE_H

#include <ArduinoJson.h>

void queueEvent(JsonDocument& event);
void resendQueuedEvents();

#endif
