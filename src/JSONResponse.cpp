#pragma once
#include <esp_http_server.h>

esp_err_t SendStatusOK(httpd_req_t *request) {
    httpd_resp_set_type(request, "application/json");
    httpd_resp_sendstr(request, "{\"status\":\"ok\"}");
    return ESP_OK;
}

esp_err_t SendStatusError(httpd_req_t *request, const char* description) {
    httpd_resp_set_type(request, "application/json");
    httpd_resp_send_chunk(request, "{\"status\":\"error\", \"error\": \"", HTTPD_RESP_USE_STRLEN);
    // NOTE: We silently assume that description does not have any special characters
    httpd_resp_send_chunk(request, description, HTTPD_RESP_USE_STRLEN);
    httpd_resp_send_chunk(request, "\"}", HTTPD_RESP_USE_STRLEN);
    httpd_resp_send_chunk(request, nullptr, 0); // Finished
    return ESP_OK;
}