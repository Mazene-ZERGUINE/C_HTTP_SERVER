#include "../../includes/client_request_handler.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include "../includes/memory_utils.h"

#include "../includes/http_request_parser.h"

void handle_client_request(int client_file_descriptor, struct sockaddr_in client_address) {
    if (client_file_descriptor < 0) {
        fprintf(stderr, "Failed to accept client connection \n");
    }

    printf("Client connection established\n");
    printf("Client IP: %s \n", inet_ntoa(client_address.sin_addr));
    printf("Client port: %d\n", client_address.sin_port);

    // Allocating buffer to read request initial request with initial size of 4kb
    char *buffer = malloc(BUFFER_SIZE);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for buffer \n");
        close(client_file_descriptor);
        return;
    }

    // reading the request raw string and reallocating buffer size dynamically if needed
    ssize_t bytes_received = recv(client_file_descriptor, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > BUFFER_SIZE - 1) {
        realloc_buffer(&buffer, bytes_received);
    }

    if (bytes_received < 0) {
        perror("Error receiving data");
        close(client_file_descriptor);
        return;
    } else if (bytes_received == 0) {
        printf("Client disconnected.\n");
        close(client_file_descriptor);
        return;
    }


    HttpRequest *http_request = parse_http_request(buffer);
    if (http_request == NULL) {
        fprintf(stderr, "Failed to parse HTTP request.\n");
        close(client_file_descriptor);
        return;
    }

    // TODO: More approprite Login handling //
    printf("\n--- Parsed HTTP Request ---\n");
    printf("Method: %s\n", http_method_to_string(http_request->http_method));
    printf("Path: %s\n", http_request->path);
    printf("HTTP Version: %s\n", http_request->http_version);
    printf("Headers count: %d\n", http_request->header_count);
    printf("Headers: [");
    for (int i = 0; i < http_request->header_count; i++) {
        printf("%s : %s \n", http_request->headers[i].key, http_request->headers[i].value);
    }
    printf("]\n");


    printf("\n---------------------------\n");

    close(client_file_descriptor);
    free(http_request);
}
