#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

/**
 * @file http_response.h
 * @brief Defines the HttpResponse structure and functions for handling HTTP responses.
 *
 * This header file contains declarations for constructing, formatting, and managing HTTP responses.
 * It includes status codes, headers, and response body management.
 */

#include "http_request_parser.h"
#include <string.h>

/**
 * @struct HttpStatus
 * @brief Represents an HTTP status code and its corresponding reason phrase.
 */
typedef struct {
    int status_code;        /**< The numeric HTTP status code (e.g., 200, 404, 500) */
    const char *reason_phrase; /**< The associated reason phrase (e.g., "OK", "Not Found") */
} HttpStatus;

/**
 * @brief A list of standard HTTP status codes and their corresponding reason phrases.
 */
static const HttpStatus HTTP_STATUS_LIST[] = {
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {204, "No Content"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {304, "Not Modified"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {503, "Service Unavailable"},
};

/**
 * @struct HttpResponse
 * @brief Represents an HTTP response.
 *
 * This structure stores the HTTP response details, including:
 * - HTTP version (e.g., "HTTP/1.1").
 * - Status code and reason phrase.
 * - Headers.
 * - Response body.
 */
typedef struct {
    const char *http_version; /**< HTTP version (e.g., "HTTP/1.1") */
    int status_code;          /**< Numeric HTTP status code */
    const char *reason_phrase; /**< Corresponding reason phrase */
    HttpHeader *headers;      /**< Pointer to an array of headers */
    int header_count;         /**< Number of headers in the response */
    char *body;               /**< Response body content */
} HttpResponse;

/**
 * @brief Creates and initializes a new HttpResponse instance.
 *
 * Allocates memory for an HttpResponse structure and sets default values.
 *
 * @return Pointer to a new HttpResponse structure.
 */
HttpResponse* http_response_new(void);

/**
 * @brief Builds an HTTP response with a given status code and optional body.
 *
 * Populates the HttpResponse structure with the given status and body.
 *
 * @param http_response Pointer to the HttpResponse structure.
 * @param status_code The HTTP status code (e.g., 200, 404).
 * @param body Optional body content (set to NULL for empty responses).
 */
void build_http_response(HttpResponse* http_response, int status_code, const char* body);

/**
 * @brief Adds a header to the HTTP response.
 *
 * Stores a new HTTP header key-value pair in the HttpResponse structure.
 *
 * @param http_response Pointer to the HttpResponse structure.
 * @param key The header name (e.g., "Content-Type").
 * @param value The header value (e.g., "text/html").
 */
void add_http_response_header(HttpResponse* http_response, const char* key, const char* value);

/**
 * @brief Converts an HttpResponse structure into a properly formatted HTTP response string.
 *
 * Formats the HTTP response with headers and body as a complete HTTP message.
 *
 * @param http_response Pointer to the HttpResponse structure.
 * @return A dynamically allocated string representing the full HTTP response (must be freed by the caller).
 */
char* http_response_to_string(HttpResponse* http_response);

/**
 * @brief Frees the allocated memory of an HttpResponse structure.
 *
 * Releases all dynamically allocated memory used within the HttpResponse.
 *
 * @param response Pointer to the HttpResponse structure to be freed.
 */
void free_http_response(HttpResponse* response);

/**
 * @brief Retrieves the reason phrase associated with a given HTTP status code.
 *
 * Searches the HTTP status list for a matching code and returns the corresponding phrase.
 *
 * @param status_code The HTTP status code to look up.
 * @return The corresponding reason phrase, or NULL if not found.
 */
const char* get_http_reason_phrase(int status_code);

/**
 * @brief Retrieves the HTTP status code from a reason phrase.
 *
 * Searches the HTTP status list for a matching reason phrase and returns the corresponding code.
 *
 * @param reason_phrase The reason phrase to look up (e.g., "Not Found").
 * @return The corresponding HTTP status code, or -1 if not found.
 */
int get_http_status_code_from_phrase(const char* reason_phrase);

#endif // HTTP_RESPONSE_H
