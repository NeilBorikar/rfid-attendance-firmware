#include "time_utils.h"
#include <time.h>

unsigned long getUnixTimestamp() {
    return time(nullptr);
}
