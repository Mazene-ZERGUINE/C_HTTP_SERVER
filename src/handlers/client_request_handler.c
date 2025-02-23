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
    if (client_file_descriptor < 0) {
        log_warn("Failed to accept client connection from client %s", inet_ntoa(client_address.sin_addr));
    }

    log_info("Connection established from client");
    log_info("Client IP: %s", inet_ntoa(client_address.sin_addr));
    log_info("Client port: %d", client_address.sin_port);

    char *buffer = malloc(BUFFER_SIZE);
    if (!buffer) {
        log_error("Failed to allocate memory for buffer");
        close(client_file_descriptor);
        return;
    }

    ssize_t bytes_received = recv(client_file_descriptor, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > BUFFER_SIZE - 1) {
        realloc_buffer(&buffer, bytes_received);
    }

    if (bytes_received < 0) {
        log_info("Error receiving data from client %s", inet_ntoa(client_address.sin_addr));
        close(client_file_descriptor);
        free(buffer);
        return;
    } else if (bytes_received == 0) {
        log_info("Client %s disconnected", inet_ntoa(client_address.sin_addr));
        close(client_file_descriptor);
        free(buffer);
        return;
    }

    HttpRequest *http_request = parse_http_request(buffer);
    free(buffer);  // ✅ Free buffer after parsing

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
    HttpResponse *http_response = handel_http_response(http_request, server, &response_length, &response_body);

    char *response_headers = http_response_to_string(http_response);

    send_response(client_file_descriptor, response_headers, response_body, response_length);

    free_http_response(http_response);
    free(response_headers);
    free(response_body);
}
