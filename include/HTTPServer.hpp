#pragma once

#include "HTTPServer.hpp"
#include <esp_http_server.h>

/**
 * HTTP server.
 * Works with ESP-IDF library internally.
 * 
 * Usage:
 * First call http.StartServer()
 * Then call http.RegisterHandler(...) for all handlers
 */
class HTTPServer {
public:
    HTTPServer();

    /**
     * @brief Starts the HTTP server
     * This will call httpd_start internally,
     * which will in turn start a separate task running the server.
     */
    void StartServer();

    /**
     * @brief Registers a handler for the HTTP server
     * This is typically called multiple times.
     * Note that the handler is not copied, so it must be kept in scope.
     * Typically, declare const handler objects globally.
     */
    void RegisterHandler(const httpd_uri_t *uri_handler);

    httpd_handle_t server = nullptr;
    httpd_config_t conf;
};