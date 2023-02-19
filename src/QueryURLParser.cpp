#include "QueryURLParser.hpp"

QueryURLParser::QueryURLParser(httpd_req_t *req) {
    size_t queryURLLen = httpd_req_get_url_query_len(req) + 1;

    if(queryURLLen > 1) {
        // Allocate filled with \0
        this->queryURLString = std::string(queryURLLen, '\0');
        if (httpd_req_get_url_query_str(req, queryURLString.data(), queryURLLen) != ESP_OK) {
            ESP_LOGE("Query URL parser", "Failed to extract query URL");
            // Set string to "not found"
            this->queryURLString = "";
        }
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
    if(err != ESP_OK) {
        ESP_LOGE("Query URL parser", "parsing URL");
        Serial.println(esp_err_to_name(err));
        free(buf);
        return "";
    }
    // Convert to std::string
    std::string param(buf); // by strlen
    free(buf);
    // Shrink param so it fits.
    return param;
}

std::string queryURLString;
