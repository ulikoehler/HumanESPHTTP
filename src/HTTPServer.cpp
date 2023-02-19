#include "HTTPServer.hpp"

HTTPServer::HTTPServer(): conf(HTTPD_DEFAULT_CONFIG()) {
}

void HTTPServer::StartServer() {
    if (httpd_start(&server, &conf) != ESP_OK) {
        ESP_LOGE("HTTP server", "Error starting server!");
    }
}

void HTTPServer::RegisterHandler(const httpd_uri_t *uri_handler) {
    httpd_register_uri_handler(this->server, uri_handler);
}