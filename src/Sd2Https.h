#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "Sd2Http.h"

namespace sd2 {

// HTTPS GET 客户端：统一 BearSSL 证书、Bearer 鉴权与响应读取。
class Https {
public:
    // pem: PROGMEM 根证书；verifyTlsCert=false 时跳过证书校验
    Https(const char *pem, bool verifyTlsCert = true);

    // 完成连接 + GET；out 返回 header/body；error 给中文错误描述
    bool get(const char *host, const char *path, const char *bearerToken,
             const char *userAgent, HttpResponse &out, String &error,
             uint32_t timeoutMs = 8000);

private:
    BearSSL::X509List trust_;
    bool verifyTlsCert_;
};

} // namespace sd2
