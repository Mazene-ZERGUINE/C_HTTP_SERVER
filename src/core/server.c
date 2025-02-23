/**
*  @file server.c
 * @brief Server module implimentation for handling server operations and sockets binding.
 * Created by Mazene ZERGUINZ on 21/02/2025.
 */

#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include "client_request_handler.h"
#include "config.h"
#include <fcntl.h>
#include <unistd.h>
#include "logger.h"

Server *create_server(const char *port, int max_connections) {
    int server_port = validate_server_port(port);
    Server *server = malloc(sizeof(Server));

    if (!server) {
        log_error("Server could not be started ", "Failed to allocate memory to the server");
        log_debug("Error allocating memory <create_server()> line 24", "server.c");
        exit(EXIT_FAILURE);
    }

    server->server_port = server_port;
    server->max_connections = max_connections;
    server->server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_file_descriptor < 0) {
        log_error("Server could not be started ", "Failed to connect to socket");
        log_debug("Error allocating memory <create_server() line 33>", "server.c");
        free(server);
        exit(EXIT_FAILURE);
    }

    server->server_address.sin_family = AF_INET;
    server->server_address.sin_port = htons(server_port);
    server->server_address.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    return server;
}

int validate_server_port(const char *server_port) {
    if (server_port == NULL) {
        log_info("No port specified. Using default port %d.\n", DEFAULT_PORT);
        return (int) DEFAULT_PORT;
    }

    errno = 0;
    char *enp_ptr;
    long port = strtol(server_port, &enp_ptr, 10);

    if (errno != 0 || *enp_ptr != '\0' || port <= 0 || port > 65535) {
        log_warn("Invalid port number: %s. Using default port %d.\n", server_port, DEFAULT_PORT);
        return (int) DEFAULT_PORT;
    }
    return (int) port;
}

void start_server(Server *server) {
    // binds the server to the selected port or 5400 by default
    if (bind(server->server_file_descriptor, (struct sockaddr *) &server->server_address,
             sizeof(server->server_address)) < 0) {
        log_error("Failed to bind server socket to port %d.\n", server->server_port);
        log_debug("Error while binding socket to port <start_server() line 66>", "server.c");
        exit(EXIT_FAILURE);
    }

    // listens to all connections on the server port
    if (listen(server->server_file_descriptor, server->max_connections) < 0) {
        log_error("Failed to listen on port %d.\n", server->server_port);
        log_debug("Error while listening to connections <start_server() line 73>", "server.c");
        exit(EXIT_FAILURE);
    }
    log_info("Serve started successfully");
    log_info("Server is running on %s and listing on port %d", inet_ntoa(server->server_address.sin_addr),
             ntohs(server->server_address.sin_port));

    // Sets the server in non-blocking accept mode
    fcntl(server->server_file_descriptor, F_SETFL, O_NONBLOCK);
    accept_connection(server);
}

void accept_connection(const Server *server) {
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client = accept(server->server_file_descriptor, (struct sockaddr *) &client_addr, &client_len);
        if (client < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                // No client is waiting, so avoid looping too fast
                usleep(10000); // Sleep for 10ms before trying again
                continue;
            } else {
                log_warn("Failed to accept client connection");
                continue;
            }
        }
        handle_client_request(client, client_addr);
    }
}

void stop_server(Server *server) {
    if (!server) return;
    close(server->server_file_descriptor);
    free(server);
    log_warn("Server stopped successfully");
}
