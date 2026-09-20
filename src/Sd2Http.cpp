#include "Sd2Http.h"

#include <Esp.h>

namespace sd2 {

int parseStatusCode(const String &statusLine) {
    int sp2 = statusLine.indexOf(' ');
    int sp3 = statusLine.indexOf(' ', sp2 + 1);
    if (sp2 > 0 && sp3 > sp2)
        return statusLine.substring(sp2 + 1, sp3).toInt();
    return 0;
}

String dechunk(const String &s) {
    String out;
    size_t i = 0;
    while (i < s.length()) {
        int nl = s.indexOf("\r\n", i);
        if (nl < 0)
            break;
        String szStr = s.substring(i, nl);
        szStr.trim();
        long sz = strtol(szStr.c_str(), NULL, 16);
        if (sz <= 0 || (size_t)(nl + 2 + sz) > s.length())
            break;
        out += s.substring(nl + 2, nl + 2 + sz);
        i = nl + 2 + sz + 2;
    }
    return out;
}

HttpResponse readHttpResponse(Client &client, uint32_t timeoutMs) {
    HttpResponse out;
    String raw;
    raw.reserve(4096);
    uint8_t buf[128];
    uint32_t t0 = millis();
    while (millis() - t0 < timeoutMs) {
        while (client.available()) {
            int n = client.read(buf, sizeof(buf));
            if (n <= 0)
                break;
            raw.concat((const char *)buf, (unsigned int)n);
        }
        if (!client.connected() && client.available() == 0)
            break;
        delay(1);
        ESP.wdtFeed();
    }

    int hEnd = raw.indexOf("\r\n\r\n");
    int sepLen = 4;
    if (hEnd < 0) {
        hEnd = raw.indexOf("\n\n");
        sepLen = 2;
    }
    if (hEnd < 0)
        return out; // 不完整响应

    out.headers = raw.substring(0, hEnd);
    out.body = raw.substring(hEnd + sepLen);
    int nl = out.headers.indexOf('\n');
    out.statusLine = (nl >= 0) ? out.headers.substring(0, nl) : out.headers;
    out.statusLine.trim();
    out.httpCode = parseStatusCode(out.statusLine);
    if (out.headers.indexOf("chunked") >= 0)
        out.body = dechunk(out.body);
    out.complete = true;
    return out;
}

bool httpGet(const char *host, uint16_t port, const char *path,
             HttpResponse &out, uint32_t timeoutMs, const char *extraHeaders) {
    WiFiClient client;
    if (!client.connect(host, port))
        return false;
    client.setNoDelay(true);

    String req = String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + host + ":" + String(port) + "\r\n" +
                 "Connection: close\r\n";
    if (extraHeaders)
        req += extraHeaders;
    req += "\r\n";
    client.print(req);

    out = readHttpResponse(client, timeoutMs);
    client.stop();
    return out.complete && out.body.length() > 0;
}

} // namespace sd2
