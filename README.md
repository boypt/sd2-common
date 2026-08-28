# SD2Common · SD2 小电视固件公共库

为 [SD2 小电视](https://oshwhub.com/Q21182889/esp-xiao-dian-shi)（ESP8266 + ST7789 240×240）系列固件提炼的公共基础库。

## 模块

| 头文件 | 内容 |
|--------|------|
| `Sd2Wifi.h` | WiFi STA 连接状态机：自动重连、连接/断开边沿 |
| `Sd2Time.h` | NTP 校时、本地小时、本地时间格式化 |
| `Sd2Sleep.h` | 跨午夜休眠窗口判断 + 休眠状态机（进入/退出回调） |
| `Sd2Backlight.h` | 背光控制：反相 PWM / 正相 PWM / 低电平点亮（0~1023 越大越亮） |
| `Sd2Http.h` | 轻量 HTTP：读响应、拆 header/body、解析状态码、解 chunked、明文 GET |
| `Sd2Https.h` | HTTPS GET：统一 BearSSL 证书校验、Bearer 鉴权、响应读取 |
| `Sd2Format.h` | 数字字符串清理、速率格式化（64px 大数字） |
| `Sd2Theme.h` | 四个工程统一的暗色主题 RGB565 配色 |
| `Sd2App.h` | 公共运行骨架：WiFi/NTP/休眠/背光/堆内存打印，各工程挂业务回调 |

## 接入方式

工程与 `sd2-common` 平级存放，然后在 `platformio.ini` 里添加本地依赖：

```ini
lib_deps =
    ../sd2-common
```

公共的 ST7789 板级配置（平台、引脚、字体内置宏）也可以直接引入：

```ini
[platformio]
extra_configs = ../sd2-common/platformio/sd2-st7789.ini
```

工程自己的 `[env:nodemcuv2]` 会与公共片段自动合并，只需保留自身差异（额外的字体宏、依赖）。

屏幕驱动、SPI 引脚、分辨率等硬件常量已固化在 [`platformio/tft_setup.h`](platformio/tft_setup.h)，TFT_eSPI 会通过 `-I` 路径自动加载（`${common.sd2_flags}` 指向它）。硬件固定后这些不属于用户配置，新工程无需再写一长串 `-D` 宏。

## 使用示例

### WiFi 连接

```cpp
#include <SD2Common.h>

static sd2::Wifi wifi;

void setup() {
    wifi.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    wifi.loop();                       // 断线时每 5 秒自动重试
    if (wifi.justConnected()) {
        Serial.printf("IP: %s\n", wifi.ip().c_str());
    }
    if (!wifi.connected()) return;
    // ...
}
```

### 定时休眠（支持跨午夜）

```cpp
static sd2::SleepScheduler sleepSched(SLEEP_START_HOUR, SLEEP_END_HOUR);
static sd2::Backlight backlight(TFT_BL, sd2::Backlight::PWM_INVERTED);

void loop() {
    // 用当前本地小时驱动状态机，进入/退出时回调
    if (sleepSched.update(sd2::localHour())) {
        if (sleepSched.sleeping()) backlight.off();
        else backlight.on();
    }
    // 休眠期间可以用更长的轮询间隔，到点自动唤醒
    uint32_t interval = sleepSched.interval(POLL_INTERVAL_MS, 30000UL);
}
```

### HTTP / HTTPS

```cpp
// 明文 GET（openwrt 数据接口）
sd2::HttpResponse resp;
if (sd2::httpGet("192.168.1.1", 80, "/cgi-bin/traffic", resp, 4000)) {
    // resp.body 已解 chunked；resp.httpCode 为状态码
}

// HTTPS GET（带证书校验）
static sd2::Https https(API_ROOT_CERT_PEM);
sd2::HttpResponse resp;
String error;
if (https.get("api.example.com", "/data", API_KEY,
              "SD2/1.0", resp, error, 8000)) {
    // resp.body 为 JSON 原文，由各工程自行解析
}
```

## 兼容性

- PlatformIO + Arduino framework，板级 `espressif8266`（`nodemcuv2`）。
- `sd2::readHttpResponse` 接受任意 `Client`（`WiFiClient` / `WiFiClientSecure`）。
- 除 `Sd2App.h` 需要 TFT_eSPI 外，其余模块均为纯函数/轻量状态机。
