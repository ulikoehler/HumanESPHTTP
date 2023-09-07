#include <Arduino.h>
#include <HTTPServer.hpp>
#include <WiFi.h>

// Declare server globally
HTTPServer http;

// Example handler
static const httpd_uri_t helloWorldHandler = {
    .uri       = "/api/hello-world",
    .method    = HTTP_GET,
    .handler   = [](httpd_req_t *request) {
        httpd_resp_set_type(request, "text/plain");
        httpd_resp_sendstr(request, "Hello World!");

        return ESP_OK;
    }
};

void setup() {
    // TODO Example code
    WiFi.begin("MyWifi", "MyWifiPassword");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // ...
    // Start HTTP server
    http.StartServer();
    http.RegisterHandler(&helloWorldHandler);
}

void loop() {
    // Nothing to do here since the HTTP server
    // runs in a separate thread
    delay(1000);
}