#include "Sd2Https.h"

namespace sd2 {

Https::Https(const char *pem, bool verifyTlsCert)
    : trust_(pem), verifyTlsCert_(verifyTlsCert) {}

bool Https::get(const char *host, const char *path, const char *bearerToken,
                const char *userAgent, HttpResponse &out, String &error,
                uint32_t timeoutMs, const char *extraHeader) {
    WiFiClientSecure client;
    if (verifyTlsCert_) {
        client.setTrustAnchors(&trust_);
    } else {
        client.setInsecure();
    }
    client.setTimeout(timeoutMs); // Stream 超单位为 ms（勿除以 1000）

    uint32_t t0 = millis();
    if (!client.connect(host, 443)) {
        char sslErr[64] = {0};
        client.getLastSSLError(sslErr, sizeof(sslErr));
        Serial.printf("TLS connect failed after %lu ms, ssl=%s\n",
                      (unsigned long)(millis() - t0), sslErr);
        error = "Network error";
        client.stop();
        return false;
    }

    String req = "GET " + String(path) + " HTTP/1.1\r\n" +
                 "Host: " + String(host) + "\r\n" +
                 "Authorization: Bearer " + String(bearerToken) + "\r\n" +
                 "User-Agent: " + String(userAgent) + "\r\n" +
                 "Accept: application/json\r\n" +
                 "Connection: close\r\n";
    if (extraHeader != nullptr && extraHeader[0] != '\0') {
        req += String(extraHeader) + "\r\n";
    }
    req += "\r\n";
    client.print(req);
    client.flush();

    out = readHttpResponse(client, timeoutMs);
    client.stop();
    if (!out.complete) {
        error = "Network error";
        return false;
    }
    return true;
}

} // namespace sd2
