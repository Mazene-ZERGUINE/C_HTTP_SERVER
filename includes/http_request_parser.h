/**
 * @file http_request_parser.h
 * @brief HTTP request parsing module.
 *
 * This module provides functionalities for parsing raw HTTP request strings
 * into structured data
 *
 * Created by Mazene ZERGUINZ on 21/02/2025.
 */

#ifndef HTTP_REQUEST_PARSER_H
#define HTTP_REQUEST_PARSER_H
#define INITIAL_HEADER_CAPACITY 10


#include <stdlib.h>

/**
 * @brief Represents a single HTTP header.
 *
 * Each HTTP header consists of a key-value pair (e.g., "Content-Type: application/json").
 */
typedef struct {
    char *key;
    char *value;
} HttpHeader;

/**
 * @brief Enumeration of supported HTTP methods.
 *
 * The parser recognizes standard HTTP methods. If the method is not recognized,
 * it defaults to `HTTP_UNKNOWN`.
 */
typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_PATCH,
    HTTP_UNKNOWN
} HttpMethod;

/**
 * @brief Represents a parsed HTTP request.
 *
 * This structure holds all components of an HTTP request, including method, path,
 * HTTP version, headers, and an optional request body.
 */
typedef struct {
    HttpHeader *headers;
    HttpMethod http_method;
    char *path;
    char *http_version;
    int header_count;
} HttpRequest;

/**
 * @brief Converts an HTTP method string to an HttpMethod enum.
 *
 * This function takes a string (e.g., "GET", "POST") and returns the corresponding
 * `HttpMethod` enum value.
 *
 * @param method String representation of the HTTP method.
 * @return HttpMethod The corresponding enum value, or HTTP_UNKNOWN if unrecognized.
 */
HttpMethod get_http_method(const char *method);

/**
 * @brief Converts an HttpMethod enum to a string.
 *
 * This function returns the string representation of an HttpMethod (e.g., HTTP_GET → "GET").
 *
 * @param method HttpMethod enum value.
 * @return const char* Corresponding string representation.
 */
const char *http_method_to_string(HttpMethod method);

/**
 * @brief Parses an HTTP request from a raw string.
 *
 * This function takes a raw HTTP request string and extracts the request line, headers,
 * and body into an `HttpRequest` structure.
 *
 * @param request_str The raw HTTP request string.
 * @return HttpRequest* Pointer to the parsed HttpRequest structure, or NULL on failure.
 */
HttpRequest *parse_http_request(const char *request_str);

/**
 * @brief Parses the request line of an HTTP request.
 *
 * The request line contains the method, requested path, and HTTP version (e.g., "GET /index.html HTTP/1.1").
 *
 * @param req_copy A mutable copy of the request string.
 * @param http_request Pointer to an allocated HttpRequest structure.
 * @return int Returns 0 on success, -1 on failure.
 */
char *parse_http_request_line(const char *req_copy, HttpRequest *http_request);

/**
 * @brief Parses HTTP headers from a request.
 *
 * Extracts header key-value pairs and stores them in the HttpRequest structure.
 *
 * @param req_string A mutable copy of the request string.
 * @param http_request Pointer to an allocated HttpRequest structure.
 * @return int Returns 0 on success, -1 on failure.
 */
int parse_http_header(char *req_string, HttpRequest *http_request);

/**
 * @brief Frees the memory allocated for an HttpRequest structure.
 *
 * This function ensures all dynamically allocated memory (headers, path, version, body)
 * is properly freed.
 *
 * @param request Pointer to the HttpRequest structure.
 */
void free_http_request(HttpRequest *request);

#endif // HTTP_REQUEST_PARSER_H
