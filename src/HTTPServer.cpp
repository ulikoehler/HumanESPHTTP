#include "HTTPServer.hpp"
#include <esp_log.h>

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

void HTTPServer::SetCORSHeaders(httpd_req_t* req) {
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "*");
}