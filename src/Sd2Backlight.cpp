#include "Sd2Backlight.h"

namespace sd2 {

void Backlight::begin() {
    pinMode(pin_, OUTPUT);
}

void Backlight::setBrightness(int brightness) {
    brightness_ = constrain(brightness, 0, 1023);
    state_ = brightness_ > 0;
    switch (mode_) {
        case PWM_INVERTED:
            pinMode(pin_, OUTPUT);
            analogWrite(pin_, 1023 - brightness_);
            break;
        case PWM_NORMAL:
            pinMode(pin_, OUTPUT);
            analogWrite(pin_, brightness_);
            break;
        case SWITCH_ACTIVE_LOW:
            // 低电平点亮：亮度越大占空比越小。
            // 保留原固件驱动序列（INPUT -> analogWrite -> OUTPUT）。
            pinMode(pin_, INPUT);
            analogWrite(pin_, 1023 - brightness_);
            pinMode(pin_, OUTPUT);
            break;
    }
}

void Backlight::off() {
    state_ = false;
    switch (mode_) {
        case PWM_INVERTED:
            pinMode(pin_, OUTPUT);
            analogWrite(pin_, 1023);
            break;
        case PWM_NORMAL:
            pinMode(pin_, OUTPUT);
            analogWrite(pin_, 0);
            break;
        case SWITCH_ACTIVE_LOW:
            pinMode(pin_, OUTPUT);
            digitalWrite(pin_, HIGH);
            break;
    }
}

} // namespace sd2
