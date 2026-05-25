#include "HTTPServer.hpp"
#include <esp_err.h>
#include <esp_log.h>

HTTPServer::HTTPServer(): conf(HTTPD_DEFAULT_CONFIG()) {
    ApplyDefaultConfig();
}

void HTTPServer::ApplyDefaultConfig() {
    conf.max_uri_handlers = DefaultMaxURIHandlers;
    conf.max_resp_headers = DefaultMaxResponseHeaders;
}

void HTTPServer::StartServer() {
    if (httpd_start(&server, &conf) != ESP_OK) {
        ESP_LOGE("HTTP server", "Error starting server!");
    }
}

void HTTPServer::RegisterHandler(const httpd_uri_t *uri_handler) {
    const esp_err_t err = httpd_register_uri_handler(this->server, uri_handler);
    if (err != ESP_OK) {
        ESP_LOGE("HTTP server", "Failed to register handler %s: %s",
                 uri_handler ? uri_handler->uri : "<null>", esp_err_to_name(err));
    }
}

void HTTPServer::SetCORSHeaders(httpd_req_t* req) {
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "*");
}