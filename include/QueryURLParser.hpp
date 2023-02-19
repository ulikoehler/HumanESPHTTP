#pragma once

#include <string>
#include <esp_http_server.h>


class QueryURLParser {
public:
    QueryURLParser(httpd_req_t *req);
    std::string GetParameter(const char* key);

private:
    std::string queryURLString;
};
