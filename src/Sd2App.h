#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

#include "Sd2Backlight.h"
#include "Sd2Sleep.h"
#include "Sd2Time.h"
#include "Sd2Wifi.h"

namespace sd2 {

// SD2 公共运行骨架：WiFi/NTP/休眠/背光/堆内存打印。
// 各工程保留自己的绘制与数据逻辑，通过回调挂进来。
class App {
public:
    typedef void (*BoolFn)(bool);
    typedef void (*Fn)();

    TFT_eSPI tft;
    Wifi wifi;
    SleepScheduler sleep;
    Backlight backlight;

    // 与老工程全局变量同名，方便直接沿用现有绘制代码
    bool bootDone = false;
    bool ntpDone = false;  // NTP 同步后由 App 自动置位
    bool wifiFailShown = false;
    uint32_t bootStart = 0;
    uint32_t lastFetchMs = 0;
    uint32_t lastSleepCheck = 0;
    uint32_t lastHeapPrint = 0;

    App(uint32_t pollIntervalMs, int sleepStartHour, int sleepEndHour)
        : sleep(sleepStartHour, sleepEndHour),
          backlight(5, Backlight::PWM_INVERTED),
          pollMs_(pollIntervalMs) {}

    // 无轮询工程（如时钟）不需要伪造轮询间隔
    App(int sleepStartHour, int sleepEndHour)
        : App(0, sleepStartHour, sleepEndHour) {}

    void setHooks(BoolFn drawBootPage, Fn onTick,
                  Fn onConnected = nullptr, Fn onDisconnected = nullptr,
                  Fn onSleep = nullptr, Fn onWake = nullptr);

    // 初始化屏幕/背光/启动页/WiFi；Clock 需要先画启动页再开背光
    void begin(const char *ssid, const char *pass, int brightness,
               int32_t tzOffsetSec, const char *ntpServer,
               bool drawBootBeforeBacklight = false);

    void loop();

private:
    void handleWiFi();
    void updateSleep();

    uint32_t pollMs_;
    int32_t tzOffsetSec_ = 0;
    const char *ntpServer_ = nullptr;
    BoolFn drawBoot_ = nullptr;
    Fn tick_ = nullptr;
    Fn onConnected_ = nullptr;
    Fn onDisconnected_ = nullptr;
    Fn onSleep_ = nullptr;
    Fn onWake_ = nullptr;
};

} // namespace sd2
