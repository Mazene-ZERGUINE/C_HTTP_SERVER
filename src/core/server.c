//
// Created by Mazene ZERGUINZ on 21/02/2025.
//

#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>

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
    server->server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

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
