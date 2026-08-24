#pragma once

#include <Arduino.h>

namespace sd2 {

// 数字字符串清理："12.30" -> "12.3"、"12.00" -> "12"、空串 -> "0"
String trimNumber(const String &s);

// 速率格式化（SD2 64px 大数字，最多约 3 字符）：
// mbps 以 Mbps 传入；value 输出数值文本，unit 输出单位（Gbps/Mbps/Kbps/bps）
void formatRate(double mbps, char *value, size_t valueLen,
                char *unit, size_t unitLen);

} // namespace sd2
