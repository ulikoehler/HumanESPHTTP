# HumanESPHTTP
ESP-IDF HTTP server high-level API for humans, not for machines.
This library attempts to provide a simplified API wrapping the ESP-IDF HTTP server,
enabling you to implement REST APIs and webpages with ease.

## How to install

Add this to your `platformio.ini`:

```ini
lib_deps =
    ulikoehler/HumanESPHTTP@^1.1.0
```

## Hello world (webserver) example

[examples/hello-world.cpp](Check out the full hello world example)

```c++
#include <Arduino.h>
#include <HTTPServer.hpp>

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
    // TODO setup wifi or Ethernet
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
```

## Query URL parser example

*Note*: The query URL parser does not perform URL decoding.

[examples/query-parser.cpp](Check out the full query parser example)

```c++
#include <Arduino.h>
#include <HTTPServer.hpp>
#include <QueryURLParser.hpp>

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
            httpd_resp_set_status(request, "400 Bad Request");
            httpd_resp_sendstr(request, "No 'param' query parameter found!");
        }
        return ESP_OK;
    }
};
```

### Query URL parser with `float` parameter example

```c++

static const httpd_uri_t setPowerHandler = {
    .uri       = "/api/set-power",
    .method    = HTTP_GET,
    .handler   = [](httpd_req_t *request) {
        QueryURLParser parser(request);
        httpd_resp_set_type(request, "text/plain");
        if(parser.HasParameter("power")) {
            std::string power = parser.GetParameter("power");
            // Parse power as float
            float powerFloat;
            try {
              powerFloat = std::stof(power);
            } catch (const std::invalid_argument& e) {
              httpd_resp_set_type(request, "text/plain");
              httpd_resp_set_status(request, "400 Bad Request");
              httpd_resp_sendstr(request, "Error: Invalid argument for power parameter (not a float)!");
              return ESP_OK;
            }
            // TODO Your code goes here!
            // Example code: send back power
            httpd_resp_send_chunk(request, "Power is: ", HTTPD_RESP_USE_STRLEN);
            httpd_resp_send_chunk(request, std::to_string(powerFloat).c_str(), HTTPD_RESP_USE_STRLEN);
            httpd_resp_send_chunk(request, nullptr, 0); // Finished
        } else {
            httpd_resp_set_type(request, "text/plain");
            httpd_resp_set_status(request, "400 Bad Request");
            httpd_resp_sendstr(request, "No 'power' query parameter found!");
        }
        return ESP_OK;
    }
};
```

### ArduinoJson response serialization example

```cpp
static const httpd_uri_t valueHandler = {
    .uri       = "/api/value",
    .method    = HTTP_GET,
    .handler   = [](httpd_req_t *req) {
        httpd_resp_set_type(req, "application/json");
        // create json docuemnt
        DynamicJsonDocument json(1024);
        json["value"] = 1.0;
        // Serialize JSON to string
        std::string buf;
        serializeJson(json, buf);
        // Send response
        httpd_resp_send(req, buf.c_str(), buf.length());
        return ESP_OK;
    }
};
```

## Utility functions

```c++

/**
 * Utility function, handles a single query parameter,
 * parses it as float and responds with JSON
 * in case of error.
 * 
 * In case of success, the callback is called with the
 * parsed float value and is expected to respond
 * 
 * @param paramName 
 * @param request 
 * @param json 
 */
esp_err_t HandleSingleFloatQueryParam(const char* paramName, httpd_req_t *request, std::function<void(float, httpd_req_t *request)> callback) {
    QueryURLParser parser(request);
    if(parser.HasParameter(paramName)) {
        httpd_resp_set_type(request, "text/plain");
        std::string paramStr = parser.GetParameter(paramName);
        if(paramStr.empty()) {
            httpd_resp_set_type(request, "application/json");
            httpd_resp_set_status(request, "400 Bad Request");
            std::string response = std::string(
                "{\"status\": \"error\", \"msg\": \"Error: Invalid argument for"
            ) + paramName + " parameter (string is empty)!\"}";
            httpd_resp_sendstr(request, response.c_str());
            return ESP_OK;
        }

        // Parse threshold as float
        float paramFloat;
        try {
            paramFloat = std::stof(paramStr);
            // Parsed successfully, set in NVS
            callback(paramFloat, request);
        } catch (const std::invalid_argument& e) {
            httpd_resp_set_type(request, "application/json");
            httpd_resp_set_status(request, "400 Bad Request");
            std::string response = std::string(
                "{\"status\": \"error\", \"msg\": \"Error: Invalid argument for"
            ) + paramName + " parameter (not a float)!\"}";
            httpd_resp_sendstr(request, response.c_str());
            return ESP_OK;
        }
        httpd_resp_send_chunk(request, "{\"status\": \"ok\"}", HTTPD_RESP_USE_STRLEN);
        httpd_resp_send_chunk(request, nullptr, 0); // Finished
    } else {
        httpd_resp_set_type(request, "application/json");
        httpd_resp_set_status(request, "400 Bad Request");
        std::string response = std::string(
            "{\"status\": \"error\", \"msg\": \"Error: No '"
        ) + paramName + "' query parameter found!\"}";
        httpd_resp_sendstr(request, response.c_str());
    }
    return ESP_OK;
}
```

## More examples

* [ESP32 HTTP float query parser with range check example](https://techoverflow.net/2023/09/30/esp32-http-float-query-parser-with-range-check-example-using-humanesphttp/)
* [ESP-IDF (no Arduino) example](https://github.com/ulikoehler/HumanESPHTTP/blob/master/examples/esp-idf-hello-world)
* [ESP32 HTTP POST with JSON body using ArduinoJson](https://techoverflow.net/2023/11/28/esp32-http-post-with-json-body-using-arduinojson/)
* [ESP-IDF HTTP webserver minimal ArduinoJson serialization example](https://techoverflow.net/2023/03/09/esp-idf-http-webserver-minimal-arduinojson-serialization-example/)

## Sources

This library originates from two of my blogposts on [techoverflow.net](https://techoverflow.net)
- [Easy-to-use ESP32 IDF webserver C++ wrapper class
](https://techoverflow.net/2023/06/13/easy-to-use-esp32-idf-webserver-c-wrapper-class/)
- [How to parse query URLs using ESP-IDF webserver on ESP32](https://techoverflow.net/2023/06/13/how-to-parse-query-urls-using-esp-idf-webserver-on-esp32/)
- [ESP-IDF webserver: How to respond with JSON success or error message
](https://techoverflow.net/2023/06/13/esp-idf-webserver-how-to-respond-with-json-success-or-error-message/)
