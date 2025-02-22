/**
 * @file server.h
 * @brief Server module interface for handling server operations and sockets binding.
 *
 * This module defines the Server structure and its associated functions for managing
 * a network server's socket details
 *
 * Created by Mazene ZERGUINZ on 21/02/2025.
 */

#ifndef SERVER_H
#define SERVER_H
#include <netinet/in.h>

/**
 * @brief Structure representing a network server.
 *
 * This structure holds information related to the server's socket configuration and state.
 */
typedef struct Server {
    int server_port;
    int server_file_descriptor;
    int max_connections;
    struct sockaddr_in server_address;
} Server;

/**
 * @brief Validates the provided server port.
 *
 * This function checks whether the given port string represents a valid port number.
 *
 * @param port A string representing the port number.
 * @return int Returns 1 if the port is valid, 0 otherwise.
 */
int validate_server_port(const char *port);

/**
 * @brief Creates and initializes a new Server instance.
 *
 * This function allocates and sets up a Server structure with the specified port and maximum connections.
 *
 * @param port A string representing the port number. It must be validated before use.
 * @param max_connections Maximum number of concurrent connections allowed.
 * @return Server* Pointer to the newly allocated Server structure, or NULL if creation fails.
 */
Server *create_server(const char *port, int max_connections);

/**
 * @brief Starts the server.
 *
 * This function configures the server socket by binding it to the provided address and port,
 * and sets it to listen for incoming connections.
 * Note: that the actual connection handling is managed by a separate module (handlers).
 *
 * @param server Pointer to the Server instance.
 * @return int Returns 0 on success, or an error code on failure.
 */
void start_server(Server *server);

/**
 * @brief Accept oncoming connections to the server
 * this function accepts than handel all oncoming connections to the open server port
 *
 * @param server Pointer to the Server instance.
 */
void accept_connection(const Server *server);

/**
 * @brief Stops the server.
 *
 * This function stops the server by closing the server socket and performing any necessary cleanup.
 *
 * @param server Pointer to the Server instance.
 * @return int Returns 0 on success, or an error code on failure.
 */
void stop_server(Server *server);


#endif // SERVER_H
