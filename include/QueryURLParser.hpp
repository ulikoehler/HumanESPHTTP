#pragma once

#include <string>
#include <esp_http_server.h>

#ifndef _CPP17_AVAILABLE
#define _CPP17_AVAILABLE (__cplusplus >= 201703L)
#endif

#ifndef _CPP11_AVAILABLE
#define _CPP11_AVAILABLE (__cplusplus >= 201103L)
#endif

// Define HUMANESPHTTP_EXCEPTIONS to false to disable exception-based handling 
#ifndef HUMANESPHTTP_EXCEPTIONS
#define HUMANESPHTTP_EXCEPTIONS false
#endif

// Define a custom exception QueryURLParserException
#if HUMANESPHTTP_EXCEPTIONS
#include <exception>

class QueryURLParserException : public std::exception {
public:
    QueryURLParserException(const char* message) : message(message) {}
    QueryURLParserException(const std::string& message) : message(message) {}
    QueryURLParserException(const std::string&& message) : message(message) {}
    const char* what() const noexcept override { return message.c_str(); }

    /**
     * Respond to the given request with a 400 Bad Request error
     */
    inline void Respond(httpd_req_t *req) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, message.c_str());
    }

private:
    std::string message;
};

/**
 * Notifies the user that a given parameter can't be found in the query URL
*/
class QueryURLParameterNotFoundException : public QueryURLParserException {
public:
    QueryURLParameterNotFoundException(const char* message) : QueryURLParserException(message) {}
    QueryURLParameterNotFoundException(const std::string& message) : QueryURLParserException(message) {}
    QueryURLParameterNotFoundException(const std::string&& message) : QueryURLParserException(message) {}
};

/**
 * Notifies the user that a given parameter can't be converted to the requested type
 * (e.g. trying to convert an empty string to int etc)
*/
class QueryURLParameterConversionException : public QueryURLParserException {
public:
    QueryURLParameterConversionException(const char* message) : QueryURLParserException(message) {}
    QueryURLParameterConversionException(const std::string& message) : QueryURLParserException(message) {}
    QueryURLParameterConversionException(const std::string&& message) : QueryURLParserException(message) {}
};
#endif

#if _CPP17_AVAILABLE
#include <optional>
#endif

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

    #if HUMANESPHTTP_EXCEPTIONS
    /**
     * Get a given parameter, or throw a
     * QueryURLParameterNotFoundException if it does not exist
    */
    std::string GetParameterException(const char* key);

    /**
     * Get a given parameter, or throw a
     * - QueryURLParameterNotFoundException if it does not exist
     * - QueryURLParameterConversionException if it can't be converted to an int
    */
    int GetParameterIntException(const char* key);

    /**
     * Get a given parameter, or throw a
     * - QueryURLParameterNotFoundException if it does not exist
     * - QueryURLParameterConversionException if it can't be converted to an unsigned int
    */
    unsigned int GetParameterUnsignedIntException(const char* key);

    /**
     * Get a given parameter, or throw a
     * - QueryURLParameterNotFoundException if it does not exist
     * - QueryURLParameterConversionException if it can't be converted to a long
    */
    long GetParameterLongException(const char* key);

    /**
     * Get a given parameter, or throw a
     * - QueryURLParameterNotFoundException if it does not exist
     * - QueryURLParameterConversionException if it can't be converted to an unsigned long
    */
    unsigned long GetParameterUnsignedLongException(const char* key);

    /**
     * Get a given parameter, or throw a
     * - QueryURLParameterNotFoundException if it does not exist
     * - QueryURLParameterConversionException if it can't be converted to a short
    */
    float GetParameterFloatException(const char* key);
    #endif

    #if _CPP17_AVAILABLE
    /**
     * @brief Get the value of the parameter with the given key, if it exists
     * Otherwise, return an empty optional
    */
    std::optional<std::string> GetParameterOptional(const char* key);

    /**
     * @brief Get the integer value of the parameter with the given key, if it exists
     * 
     * If the parameter does not exist, return an empty optional
     * If the int can't be parsed, return an empty optional
     */
    std::optional<int> GetParameterIntOptional(const char* key);

    /**
     * @brief Get the unsigned integer value of the parameter with the given key, if it exists
     * 
     * If the parameter does not exist, return an empty optional
     * If the int can't be parsed, return an empty optional
     */
    std::optional<unsigned int> GetParameterUnsignedIntOptional(const char* key);

    /**
     * @brief Get the integer value of the parameter with the given key, if it exists
     * 
     * If the parameter does not exist, return an empty optional
     * If the long can't be parsed, return an empty optional
     */
    std::optional<long> GetParameterLongOptional(const char* key);

    /**
     * @brief Get the integer value of the parameter with the given key, if it exists
     * 
     * If the parameter does not exist, return an empty optional
     * If the long can't be parsed, return an empty optional
     */
    std::optional<unsigned long> GetParameterUnsignedLongOptional(const char* key);

    /**
     * @brief Get the integer value of the parameter with the given key, if it exists
     * 
     * If the parameter does not exist, return an empty optional
     * If the int can't be parsed, return an empty optional
     */
    std::optional<float> GetParameterFloatOptional(const char* key);
    #endif

private:
    std::string queryURLString;
};
