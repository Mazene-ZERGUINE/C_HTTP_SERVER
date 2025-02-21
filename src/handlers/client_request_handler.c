//
// Created by Mazene ZERGUINZ on 21/02/2025.
//

#include "../../includes/client_request_handler.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "../includes/http_request_parser.h"

#define BUFFER_SIZE 4096

void handel_client_request(const int client_file_descriptor) {
    printf("Waiting for client request ...\n");

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    ssize_t bytes_received = recv(client_file_descriptor, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("Error receiving data");
        close(client_file_descriptor);
        return;
    } else if (bytes_received == 0) {
        printf("Client disconnected.\n");
        close(client_file_descriptor);
        return;
    }

    buffer[bytes_received] = '\0';
    printf("Raw HTTP Request:\n%s\n", buffer);

    HttpRequest *http_request = parse_http_request(buffer);
    if (http_request == NULL) {
        fprintf(stderr, "Failed to parse HTTP request.\n");
        close(client_file_descriptor);
        return;
    }

    printf("\n--- Parsed HTTP Request ---\n");
    printf("Method: %s\n", http_method_to_string(http_request->http_method));
    printf("Path: %s\n", http_request->path);
    printf("HTTP Version: %s\n", http_request->http_version);

    printf("Headers:\n");
    for (int i = 0; i < http_request->header_count; i++) {
        printf("%s: %s\n", http_request->headers[i].key, http_request->headers[i].value);
    }
    printf("\n---------------------------\n");

    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 20\r\n"
        "\r\n"
        "Hello from server!\n";

    send(client_file_descriptor, response, strlen(response), 0);

    close(client_file_descriptor);
    free(http_request);
}
