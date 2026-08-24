#include "Sd2Wifi.h"

namespace sd2 {

void Wifi::begin(const char *ssid, const char *pass, bool persistent) {
    ssid_ = ssid;
    pass_ = pass;
    persistent_ = persistent;
    WiFi.persistent(persistent);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_, pass_);
    lastAttempt_ = millis();
}

void Wifi::loop(uint32_t now, uint32_t retryMs) {
    justConnected_ = false;
    justDisconnected_ = false;

    if (WiFi.status() == WL_CONNECTED) {
        if (!connected_) {
            connected_ = true;
            justConnected_ = true;
        }
        return;
    }

    if (connected_) {
        connected_ = false;
        justDisconnected_ = true;
    }

    if (now - lastAttempt_ >= retryMs) {
        lastAttempt_ = now;
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid_, pass_);
    }
}

} // namespace sd2
