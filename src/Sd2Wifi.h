#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

namespace sd2 {

// ESP8266 STA 连接状态机：
//  - begin() 后自动开始连接
//  - 每轮 loop() 调用一次，断线时每 retryMs 自动重试
//  - 通过 justConnected() / justDisconnected() 感知状态边沿
class Wifi {
public:
    Wifi() = default;

    // ssid/pass 必须指向长期有效的字符串（如 config.h 里的常量）
    void begin(const char *ssid, const char *pass, bool persistent = false);

    // 每次 loop 调用；now 默认 millis()
    void loop(uint32_t now = millis(), uint32_t retryMs = 5000);

    bool connected() const { return connected_; }
    bool justConnected() const { return justConnected_; }
    bool justDisconnected() const { return justDisconnected_; }
    String ip() const { return WiFi.localIP().toString(); }

private:
    const char *ssid_ = nullptr;
    const char *pass_ = nullptr;
    bool persistent_ = false;
    bool connected_ = false;
    bool justConnected_ = false;
    bool justDisconnected_ = false;
    uint32_t lastAttempt_ = 0;
};

} // namespace sd2
