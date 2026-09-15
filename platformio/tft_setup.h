#pragma once

// SD2 小电视固定硬件配置（ST7789 240x240，无 CS，MISO 未接）
// 由 TFT_eSPI 通过 __has_include(<tft_setup.h>) 自动加载，
// 所有 SD2 固件工程共用；硬件固定后不属于用户配置，无需修改。

// ---- 驱动与分辨率 ----
// SD2 的 1.54" 240x240 方屏（ST7789 初始化子集变体）必须用 ST7789_2_DRIVER：
// 完整 ST7789_DRIVER 的初始化会下发 ST7789V 的电源/伽马/时序参数并设置
// 240x320 地址窗口，该屏不接受，导致背光常亮但屏幕全黑。
#define ST7789_2_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

// ---- SPI 引脚（NodeMCU 丝印 -> GPIO）----
#define TFT_CS   -1   // 无 CS
#define TFT_DC    0   // D3
#define TFT_RST   2   // D4
#define TFT_MOSI 13   // D7
#define TFT_SCLK 14   // D5

#define SPI_FREQUENCY 40000000

// ---- 内置基础字体（各工程按需再开大字体）----
#define LOAD_GLCD
#define LOAD_FONT2

// 本板无触摸屏，消除 TOUCH_CS 提示
#define TOUCH_CS -1
