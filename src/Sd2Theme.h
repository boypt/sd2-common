#pragma once

#include <Arduino.h>

namespace sd2 {

// TFT_eSPI 同规则 RGB565 转换，供全局常量使用
inline uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

} // namespace sd2

// 四个 SD2 工程统一的暗色主题配色；各工程按需引用
#define C_BG       sd2::rgb565(0x00, 0x00, 0x00)
#define C_CARD     sd2::rgb565(0x14, 0x19, 0x26)
#define C_BORDER   sd2::rgb565(0x28, 0x32, 0x49)
#define C_SUB      sd2::rgb565(0x8A, 0x94, 0xB8)
#define C_LABEL    sd2::rgb565(0x9A, 0xA5, 0xC8)
#define C_DATE     sd2::rgb565(0x8F, 0x9B, 0xBF)
#define C_SHADOW   sd2::rgb565(0x0A, 0x0F, 0x2A)
#define C_ACCENT   sd2::rgb565(0x4D, 0x6B, 0xFE)
#define C_WHITE    sd2::rgb565(0xFF, 0xFF, 0xFF)
#define C_GREEN    sd2::rgb565(0x34, 0xD3, 0x99)
#define C_RED      sd2::rgb565(0xFF, 0x6B, 0x6B)
#define C_YELLOW   sd2::rgb565(0xF6, 0xC3, 0x43)
#define C_TEMP_HOT sd2::rgb565(0xFF, 0x5D, 0x18)
