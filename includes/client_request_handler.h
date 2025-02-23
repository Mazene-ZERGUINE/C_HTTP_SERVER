#ifndef CLIENT_REQUEST_HANDLER_H
#define CLIENT_REQUEST_HANDLER_H

#include <netinet/in.h>  // Required for `struct sockaddr_in`

#include "server.h"

/**
 * @file client_request_handler.h
 * @brief Handles incoming client requests in the server.
 *
 * This file defines the function for processing client requests
 * after they establish a connection with the server.
 * It includes necessary constants and function prototypes.
 */

#define BUFFER_SIZE 4096  /**< Buffer size for reading client requests. */

/**
 * @brief Handles an incoming request from a connected client.
 *
 * This function processes the HTTP request sent by the client
 * through the given socket file descriptor. It extracts request data,
 * processes it
 *
 * @param client_file_descriptor The file descriptor for the connected client socket.
 * @param client_address The `sockaddr_in` structure containing the client's address information.
 * @param server
 */
void handle_client_request(int client_file_descriptor, struct sockaddr_in client_address, const Server* server);

#endif // CLIENT_REQUEST_HANDLER_H
