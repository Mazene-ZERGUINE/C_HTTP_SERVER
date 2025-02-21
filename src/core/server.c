//
// Created by Mazene ZERGUINZ on 21/02/2025.
//

#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include "client_request_handler.h"
#include "config.h"

Server *create_server(const char *port, int max_connections) {
    int server_port = validate_server_port(port);
    Server *server = malloc(sizeof(Server));

    if (!server) {
        perror("Failed to allocate memory for server");
        exit(EXIT_FAILURE);
    }

    /*create a new socket and return its file descriptor (socketId)
    AF_INET => socket address family IPv4
    SOCK_STREAM => socket will use TCP
    */
    server->server_port = server_port;
    server->max_connections = max_connections;
    server->server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_file_descriptor < 0) {
        perror("Failed to create server socket");
        free(server);
        exit(EXIT_FAILURE);
    }

    // configuring the server address //
    server->server_address.sin_family = AF_INET;
    server->server_address.sin_port = htons(server_port);

    // binding server address to 127.0.0.1
    server->server_address.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    return server;
}

// validation the port set by the user using default 5200 in case or wrong port
int validate_server_port(const char *server_port) {
    if (server_port == NULL) {
        fprintf(stderr, "No port specified. Using default port %d.\n", DEFAULT_PORT);
        return DEFAULT_PORT;
    }

    errno = 0;
    char *enp_ptr;
    long port = strtol(server_port, &enp_ptr, 10);

    if (errno != 0 || *enp_ptr != '\0' || port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number: %s. Using default port %d.\n", server_port, DEFAULT_PORT);
        return (int) DEFAULT_PORT;
    }
    return (int) port;
}

void start_server(Server *server) {
    if (bind(server->server_file_descriptor, (struct sockaddr*)&server->server_address, sizeof(server->server_address)) < 0) {
        fprintf(stderr, "Failed to bind server socket to port %d.\n", server->server_port);
        exit(EXIT_FAILURE);
    }

    if (listen(server->server_file_descriptor, server->max_connections) < 0) {
        fprintf(stderr, "Failed to listen on port %d.\n", server->server_port);
        exit(EXIT_FAILURE);
    }

    printf("Server  successfully started\n");
    printf("server is running on %s on port %d\n",
           inet_ntoa(server->server_address.sin_addr),
           ntohs(server->server_address.sin_port));

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client = accept(server->server_file_descriptor, (struct sockaddr*)&client_addr, &client_len);
        if (client < 0) {
            fprintf(stderr, "Failed to accept client connection\n");
            continue;
        }
        handel_client_request(client);
    }
}

void stop_server(Server *server) {
    close(server->server_file_descriptor);
    free(server);
    printf("Server stopped.\n");
}
