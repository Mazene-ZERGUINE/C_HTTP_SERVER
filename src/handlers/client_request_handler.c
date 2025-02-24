#include "../../includes/client_request_handler.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "http_response_handler.h"
#include "logger.h"
#include "../includes/memory_utils.h"

#include "../includes/http_request_parser.h"

void handle_client_request(int client_file_descriptor, struct sockaddr_in client_address, const Server* server) {
    // no client socket ID passed to the request handling (connection failed)
    if (client_file_descriptor < 0) {
        log_warn("Failed to accept client connection from client %s", inet_ntoa(client_address.sin_addr));
    }

    log_info("Connection established from client");
    log_info("Client IP: %s", inet_ntoa(client_address.sin_addr));
    log_info("Client port: %d", client_address.sin_port);

    // allocating initial space of 4KB to read request data received from the client
    char *buffer = malloc(BUFFER_SIZE);
    if (!buffer) {
        log_error("Failed to allocate memory for buffer");
        close(client_file_descriptor);
        return;
    }

    // saving the request received data in the buffer
    const ssize_t bytes_received = recv(client_file_descriptor, buffer, BUFFER_SIZE - 1, 0);

    // reallocating space if needed
    if (bytes_received > BUFFER_SIZE - 1) {
        realloc_buffer(&buffer, bytes_received);
    }

    if (bytes_received < 0) {
        log_info("Error receiving data from client %s", inet_ntoa(client_address.sin_addr));
        close(client_file_descriptor);
        free(buffer);
        return;
    } else if (bytes_received == 0) {
        // disconnecting client if the request is empty
        log_info("Client %s disconnected", inet_ntoa(client_address.sin_addr));
        close(client_file_descriptor);
        free(buffer);
        return;
    }

    // parsing raw request data to HttpRequest strucutr
    const HttpRequest *http_request = parse_http_request(buffer);
    free(buffer);

    if (http_request == NULL) {
        log_error("Failed to parse HTTP request.");
        close(client_file_descriptor);
        return;
    }

    log_debug("Request received from client %s", inet_ntoa(client_address.sin_addr));
    log_debug("Parsing HTTP request");
    log_request(http_request);

    log_debug("Sending response to %s", inet_ntoa(client_address.sin_addr));

    size_t response_length;
    char *response_body;

    // possessing the request and Generating dynamic response content (response body, length, headers ...etc)
    HttpResponse *http_response = handel_http_response(http_request, server, &response_length, &response_body);

    // returning body and headers as plain text
    char *response_headers = http_response_to_string(http_response);

    // sending formatted response to the client with headers and content length
    send_response(client_file_descriptor, response_headers, response_body, response_length);

    // clean up
    free_http_response(http_response);
    free(response_headers);
    free(response_body);
}
