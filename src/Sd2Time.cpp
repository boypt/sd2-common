#include "Sd2Time.h"

namespace sd2 {

void timeBegin(int32_t tzOffsetSec, const char *server1, const char *server2) {
    configTime(tzOffsetSec, 0, server1, server2);
}

bool timeSynced(time_t minValidTs) {
    return time(nullptr) > minValidTs;
}

int localHour() {
    if (!timeSynced())
        return -1;
    time_t now = time(nullptr);
    struct tm tmv;
    localtime_r(&now, &tmv);
    return tmv.tm_hour;
}

String formatLocalTime(time_t t, const char *fmt) {
    struct tm tmv;
    localtime_r(&t, &tmv);
    char buf[64];
    strftime(buf, sizeof(buf), fmt, &tmv);
    return String(buf);
}

} // namespace sd2
