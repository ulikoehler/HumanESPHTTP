#include "QueryURLParser.hpp"
#include <esp_log.h>

QueryURLParser::QueryURLParser(httpd_req_t *req) {
    size_t queryURLLen = httpd_req_get_url_query_len(req) + 1;

    if(queryURLLen > 1) {
        // Allocate temporary buffer.
        // In C++17, we could write directly to the std::string using .data()
        // but we don't want to enforce C++17 yet.
        char* buf = (char*)malloc(queryURLLen);

        this->queryURLString = std::string(queryURLLen, '\0');
        if (httpd_req_get_url_query_str(req, buf, queryURLLen) != ESP_OK) {
            ESP_LOGE("Query URL parser", "Failed to extract query URL");
            // Set string to "not found"
            this->queryURLString = "";
        } else { // Parsed successfully
            this->queryURLString = std::string(buf, queryURLLen);
        }
        free(buf);
    }
}

std::string QueryURLParser::GetParameter(const char* key) {
    // Allocate enough space to store the parameter.
    // NOTE: The parameter can only be as long as the query URL itself.
    // Therefore, allocating "too much" space upfront will
    // avoid unneccessary copying
    size_t bufSize = queryURLString.size();
    char* buf = (char*)malloc(bufSize);

    /* Get value of expected key from query string */
    esp_err_t err = httpd_query_key_value(queryURLString.c_str(), key, buf, bufSize);
    if(err != ESP_OK) { // Not found
        free(buf);
        return "";
    }
    // Convert to std::string
    std::string param(buf); // by strlen
    free(buf);
    // Shrink param so it fits.
    return param;
}


bool QueryURLParser::HasParameter(const char* key) {
    char buf[1]; // Dummy buffer - will lead to truncation or OK if exists, or not found if not exists
    esp_err_t err = httpd_query_key_value(queryURLString.c_str(), key, buf, 1);
    return err == ESP_OK || err == ESP_ERR_HTTPD_RESULT_TRUNC;
}