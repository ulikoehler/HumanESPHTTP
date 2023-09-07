#pragma once

#include <string>
#include <esp_http_server.h>

class QueryURLParser {
public:
    QueryURLParser(httpd_req_t *req);

    /**
     * @brief Get the value of the parameter with the given key
     * Returns an empty string if the parameter does not exist
     * 
     * @param key The key to look for
     * @return std::string The parameter value if it exists, otherwise an empty string
     */
    std::string GetParameter(const char* key);

    /**
     * @brief Check if the query URL contains a parameter with the given key
     * 
     * @param key The key to check for
     * @return true If the query URL contains a parameter with the given key
     * @return false If the query URL does not contain a parameter with the given key
     */
    bool HasParameter(const char* key);

private:
    std::string queryURLString;
};
