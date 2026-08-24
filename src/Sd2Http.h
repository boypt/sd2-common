#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

namespace sd2 {

// 轻量 HTTP 响应，兼容 WiFiClient 与 WiFiClientSecure
struct HttpResponse {
    bool complete = false;  // 读到完整 header（httpCode 有效）
    int httpCode = 0;       // 0 = 未解析到状态码
    String statusLine;      // 如 "HTTP/1.1 200 OK"
    String headers;         // 完整 header 块
    String body;            // 已自动解 chunked
    bool ok() const { return httpCode >= 200 && httpCode < 300; }
};

// 从任意已连接的 Client 读取响应，直到连接关闭或超时。
// 自动拆分 header/body、解析状态码、解码 chunked 传输编码。
HttpResponse readHttpResponse(Client &client, uint32_t timeoutMs = 6000);

// 明文 HTTP GET。extraHeaders 以 "\r\n" 结尾追加在标准头之后。
// 连接失败/无响应返回 false；out.httpCode 有值时仍可检查具体状态码。
bool httpGet(const char *host, uint16_t port, const char *path,
             HttpResponse &out, uint32_t timeoutMs = 4000,
             const char *extraHeaders = nullptr);

// 解码 chunked 传输编码（异常时返回已解码部分）
String dechunk(const String &s);

// "HTTP/1.1 200 OK" -> 200；解析失败返回 0
int parseStatusCode(const String &statusLine);

} // namespace sd2
