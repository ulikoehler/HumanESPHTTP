#pragma once
#include <esp_http_server.h>

/**
 * Send {"status":"ok"} as JSON response
 */
esp_err_t SendStatusOK(httpd_req_t *request);

/**
 * Send {"status":"error", "error": "<description>"} as JSON response
 * 
 * NOTE: Currently, <description> is not escaped, so it must not contain any JSON special characters.
 */
esp_err_t SendStatusError(httpd_req_t *request, const char* description);