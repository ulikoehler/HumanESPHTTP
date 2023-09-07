# HumanESPHTTP
ESP-IDF HTTP server high-level API for humans, not for machines.
This library attempts to provide a simplified API wrapping the ESP-IDF HTTP server,
enabling you to implement REST APIs and webpages with ease.

## Webserver example

```c++
#include <Arduino.h>
#include <HTTPServer.h>

// Declare server globally
HTTPServer http;

// Example handler
static const httpd_uri_t helloWorldHandler = {
    .uri       = "/api/hello-world",
    .method    = HTTP_GET,
    .handler   = [](httpd_req_t *req) {
        httpd_resp_set_type(request, "text/plain");
        httpd_resp_sendstr(request, "Hello World!");

        return ESP_OK;
    }
};

void setup() {
    // TODO setup wifi or Ethernet
    http.StartServer();
    http.RegisterHandler(&helloWorldHandler);
}

void loop() {
    // Nothing to do here since the HTTP server
    // runs in a separate thread
    delay(1000);
}
```

## Query URL parser example

```c++
#include <Arduino.h>
#include <HTTPServer.h>

// Declare server globally
HTTPServer http;

// Example handler
static const httpd_uri_t helloWorldHandler = {
    .uri       = "/api/reboot",
    .method    = HTTP_GET,
    .handler   = [](httpd_req_t *req) {
        SendStatusOK(req);
        delay(20);
        ESP.restart();
        return ESP_OK;
    }
};

void setup() {
    // TODO setup wifi or Ethernet
    http.StartServer();
    http.RegisterHandler(&myHandler);
}
```

## Sources

This library originates from two of my blogposts on [techoverflow.net](https://techoverflow.net)
- [Easy-to-use ESP32 IDF webserver C++ wrapper class
](https://techoverflow.net/2023/06/13/easy-to-use-esp32-idf-webserver-c-wrapper-class/)
- [How to parse query URLs using ESP-IDF webserver on ESP32](https://techoverflow.net/2023/06/13/how-to-parse-query-urls-using-esp-idf-webserver-on-esp32/)
- [ESP-IDF webserver: How to respond with JSON success or error message
](https://techoverflow.net/2023/06/13/esp-idf-webserver-how-to-respond-with-json-success-or-error-message/)