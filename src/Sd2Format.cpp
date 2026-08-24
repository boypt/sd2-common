#include "Sd2Format.h"

namespace sd2 {

String trimNumber(const String &s) {
    String r = s;
    int dot = r.indexOf('.');
    if (dot >= 0) {
        while (r.length() > 0 && r.endsWith("0"))
            r.remove(r.length() - 1);
        if (r.endsWith("."))
            r.remove(r.length() - 1);
    }
    if (r.length() == 0)
        r = "0";
    return r;
}

void formatRate(double mbps, char *value, size_t valueLen,
                char *unit, size_t unitLen) {
    double bps = mbps * 1000000.0;
    if (bps >= 1e9) {
        snprintf(value, valueLen, "%.1f", bps / 1e9);
        snprintf(unit, unitLen, "Gbps");
    } else if (bps >= 1e7) {
        snprintf(value, valueLen, "%.0f", bps / 1e6);
        snprintf(unit, unitLen, "Mbps");
    } else if (bps >= 1e6) {
        snprintf(value, valueLen, "%.1f", bps / 1e6);
        snprintf(unit, unitLen, "Mbps");
    } else if (bps >= 1e3) {
        snprintf(value, valueLen, "%.0f", bps / 1e3);
        snprintf(unit, unitLen, "Kbps");
    } else {
        snprintf(value, valueLen, "%d", (int)bps);
        snprintf(unit, unitLen, "bps");
    }
}

} // namespace sd2
