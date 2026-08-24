#pragma once

#include <Arduino.h>
#include <time.h>

namespace sd2 {

// NTP 校时（ESP8266 无 RTC，TLS 证书校验前必须有正确的系统时间）
// tzOffsetSec: 本地时区相对 UTC 的秒偏移（东八区 = 8*3600）
void timeBegin(int32_t tzOffsetSec, const char *server1,
               const char *server2 = "pool.ntp.org");

// 系统时间是否已从 1970 纪元同步过来（超过 minValidTs 即认为有效）
bool timeSynced(time_t minValidTs = 1600000000);

// 当前本地小时（0-23）；未同步返回 -1
int localHour();

// 本地时间格式化（strftime 格式），默认 "YYYY-MM-DD HH:MM"
String formatLocalTime(time_t t, const char *fmt = "%Y-%m-%d %H:%M");

} // namespace sd2
