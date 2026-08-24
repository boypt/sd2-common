#pragma once

#include <Arduino.h>

namespace sd2 {

// SD2 背光控制，兼容两种常见硬件驱动方式：
//  - PWM_INVERTED：反相 PWM（值越大越暗，SD2 标准），off = analogWrite(pin, 1023)
//  - PWM_NORMAL：正相 PWM，off = analogWrite(pin, 0)
//  - SWITCH_ACTIVE_LOW：低电平点亮，off = 拉高；on 按亮度换算 PWM 占空比
// 三种模式语义一致：setBrightness(0..1023)，数值越大越亮。
class Backlight {
public:
    enum Mode { PWM_INVERTED, PWM_NORMAL, SWITCH_ACTIVE_LOW };

    Backlight(uint8_t pin, Mode mode) : pin_(pin), mode_(mode) {}

    void begin();

    // brightness: 0..1023，越大越亮
    void setBrightness(int brightness);
    void on() { setBrightness(brightness_); }
    void off();

    bool isOn() const { return state_; }

private:
    uint8_t pin_;
    Mode mode_;
    int brightness_ = 0;
    bool state_ = false;
};

} // namespace sd2
