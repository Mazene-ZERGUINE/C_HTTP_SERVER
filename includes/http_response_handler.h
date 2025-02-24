#ifndef HTTP_RESPONSE_HANDLER_H
#define HTTP_RESPONSE_HANDLER_H

/**
 * @file http_response_handler.h
 * @brief Defines functions for processing and sending HTTP responses.
 *
 * This header file provides the interface for handling HTTP responses,
 * including generating responses based on requests and transmitting them
 * over a network socket.
 */

#include "http-response.h"
#include "server.h"

/**
 * @brief Processes an HTTP request and generates the corresponding response.
 *
 * This function determines the appropriate response based on the given HTTP request.
 * It serves static files, handles predefined routes, and generates response headers.
 *
 * @param http_request Pointer to the parsed HttpRequest structure.
 * @param server Pointer to the Server structure containing configuration details.
 * @param response_length Pointer to store the length of the generated response body.
 * @param response_body Pointer to store the dynamically allocated response body content.
 * @return Pointer to the generated HttpResponse structure.
 */
HttpResponse* handel_http_response(const HttpRequest *http_request, const Server *server, size_t *response_length, char **response_body);

/**
 * @brief Sends an HTTP response over a socket connection.
 *
 * This function transmits HTTP headers and body content to the client through the specified file descriptor.
 * It ensures that the entire response is sent and closes the connection afterward.
 *
 * @param file_descriptor The socket file descriptor for the client connection.
 * @param headers The HTTP response headers as a formatted string.
 * @param body The HTTP response body content (may be NULL for empty responses).
 * @param body_length The length of the response body in bytes.
 */
void send_response(int file_descriptor, const char *headers, const char *body, size_t body_length);

#endif // HTTP_RESPONSE_HANDLER_H
