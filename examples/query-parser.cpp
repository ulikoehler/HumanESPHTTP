#include <Arduino.h>
#include <HTTPServer.hpp>
#include <QueryURLParser.hpp>
#include <WiFi.h>

// Declare server globally
HTTPServer http;

// Query parser example
// /api/query?param=Hello%20world!
static const httpd_uri_t queryHandler = {
    .uri       = "/api/query",
    .method    = HTTP_GET,
    .handler   = [](httpd_req_t *request) {
        QueryURLParser parser(request);
        if(parser.HasParameter("param")) {
            httpd_resp_set_type(request, "text/plain");
            std::string param = parser.GetParameter("param");
            httpd_resp_send_chunk(request, "Param is: ", HTTPD_RESP_USE_STRLEN);
            httpd_resp_send_chunk(request, param.c_str(), HTTPD_RESP_USE_STRLEN);
            httpd_resp_send_chunk(request, nullptr, 0); // Finished
        } else {
            httpd_resp_set_type(request, "text/plain");
            httpd_resp_sendstr(request, "No 'param' query parameter found!");
        }
        return ESP_OK;
    }
};

void setup() {
    // TODO setup wifi or Ethernet
    WiFi.begin("MyWifi", "MyWifiPassword");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // ...
    // Start HTTP server
    http.StartServer();
    http.RegisterHandler(&queryHandler);
}

void loop() {
    // Nothing to do here since the HTTP server
    // runs in a separate thread
    delay(1000);
}
