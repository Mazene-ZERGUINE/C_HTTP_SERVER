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

void handle_client_request(int client_file_descriptor, struct sockaddr_in client_address) {
    if (client_file_descriptor < 0) {
        log_warn("Failed to accept client connection form client", inet_ntoa(client_address.sin_addr));
    }

    log_info("Connection established from client");
    log_info("Client IP: %s \n", inet_ntoa(client_address.sin_addr));
    log_info("Client port: %d\n", client_address.sin_port);

    // Allocating buffer to read request initial request with initial size of 4kb
    char *buffer = malloc(BUFFER_SIZE);
    if (!buffer) {
        log_error("Failed to allocate memory for buffer \n");
        log_debug("Failed to allocate memory for buffer handel_client_request()");
        close(client_file_descriptor);
        return;
    }

    // reading the request raw string and reallocating buffer size dynamically if needed
    ssize_t bytes_received = recv(client_file_descriptor, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > BUFFER_SIZE - 1) {
        realloc_buffer(&buffer, bytes_received);
    }

    if (bytes_received < 0) {
        log_info("Error receiving data from client %s", inet_ntoa(client_address.sin_addr));
        close(client_file_descriptor);
        return;
    } else if (bytes_received == 0) {
        log_info("Client %s disconnected", inet_ntoa(client_address.sin_addr));
        close(client_file_descriptor);
        return;
    }


    HttpRequest *http_request = parse_http_request(buffer);
    if (http_request == NULL) {
        log_error("Failed to parse HTTP request.\n");
        log_debug("Raw HTTP request", buffer);
        close(client_file_descriptor);
        return;
    }

    log_debug("Request received from client %s", inet_ntoa(client_address.sin_addr));
    log_debug("Parsing HTTP request");
    log_request(http_request);

    log_debug("Sending response to %s", inet_ntoa(client_address.sin_addr));
    char*  response_string = handel_http_response(http_request);
    send_response(client_file_descriptor, response_string);

    close(client_file_descriptor);
    free_http_request(http_request);
}
