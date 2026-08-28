#include "Sd2App.h"

namespace sd2 {

void App::setHooks(BoolFn drawBootPage, Fn onTick,
                   Fn onConnected, Fn onDisconnected,
                   Fn onSleep, Fn onWake) {
    drawBoot_ = drawBootPage;
    tick_ = onTick;
    onConnected_ = onConnected;
    onDisconnected_ = onDisconnected;
    onSleep_ = onSleep;
    onWake_ = onWake;
}

void App::begin(const char *ssid, const char *pass, int brightness,
                int32_t tzOffsetSec, const char *ntpServer,
                bool drawBootBeforeBacklight) {
    tzOffsetSec_ = tzOffsetSec;
    ntpServer_ = ntpServer;

    tft.begin();
    tft.setRotation(0);
    tft.setTextDatum(TL_DATUM);

    if (drawBootBeforeBacklight) {
        bootStart = millis();
        if (drawBoot_) drawBoot_(false);
        backlight.begin();
        backlight.setBrightness(brightness);
    } else {
        backlight.begin();
        backlight.setBrightness(brightness);
        bootStart = millis();
        if (drawBoot_) drawBoot_(false);
    }

    wifi.begin(ssid, pass, false);
}

void App::loop() {
    handleWiFi();
    if (tick_) tick_();

    if (millis() - lastSleepCheck >= 1000) {
        lastSleepCheck = millis();
        updateSleep();
    }
    if (millis() - lastHeapPrint >= 10000) {
        lastHeapPrint = millis();
        Serial.printf("Free heap: %u B\n", ESP.getFreeHeap());
    }

    delay(20);
}

void App::handleWiFi() {
    wifi.loop();
    if (wifi.justConnected()) {
        Serial.print("WiFi connected, IP: ");
        Serial.println(wifi.ip().c_str());
        bootDone = true;
        if (onConnected_) onConnected_();
        timeBegin(tzOffsetSec_, ntpServer_);
        lastFetchMs = millis() - pollMs_; // 立即拉取/更新时间
    }
    if (wifi.justDisconnected() && onDisconnected_) {
        onDisconnected_();
    }

    if (!wifi.connected() && !bootDone && !wifiFailShown &&
        millis() - bootStart > 30000) {
        wifiFailShown = true;
        if (drawBoot_) drawBoot_(true);
    }
}

void App::updateSleep() {
    bool changed = sleep.update(localHour());
    if (changed && sleep.sleeping()) {
        backlight.off();
        Serial.println("Sleep mode: display off, fetch paused");
        if (onSleep_) onSleep_();
    } else if (changed && !sleep.sleeping()) {
        backlight.on();
        if (onWake_) onWake_();
        lastFetchMs = millis() - pollMs_;
        Serial.println("Wake up: display on");
    }
}

} // namespace sd2
