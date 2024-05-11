#include <Arduino.h>
#include <HTTPServer.hpp>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <string>

// Declare server globally
HTTPServer http;

// Example handler
static const httpd_uri_t jsonHandler = {
    .uri       = "/api/json-example",
    .method    = HTTP_GET,
    .handler   = [](httpd_req_t *request) {
        httpd_resp_set_type(req, "application/json");
        // create json docuemnt
        DynamicJsonDocument json(1024);
        json["float"] = 12.34;
        json["string"] = "ok";
        // Serialize JSON to string
        std::string buf;
        serializeJson(json, buf);
        // Send response
        httpd_resp_send(req, buf.c_str(), buf.length());
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
    http.RegisterHandler(&jsonHandler);
}

void loop() {
    // Nothing to do here since the HTTP server
    // runs in a separate thread
    delay(1000);
}