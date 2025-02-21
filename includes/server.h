//
// Created by Mazene ZERGUINZ on 21/02/2025.
//

#ifndef SERVER_H
#define SERVER_H
#include <netinet/in.h>

#define DEFAULT_PORT 5400

typedef struct Server {
    int server_port;
    int server_file_descriptor;  // ID used by the operating system to open socket
    int max_connections;
    struct sockaddr_in server_address;  // used to stock information on the server IP address (IPV4, IF_INET, PORT...)
} Server;

Server *create_server(const char* port, int max_connections);
int validate_server_port(const char *server_port);

// TODO: To be implimented later //
int start_server(Server *server);
int stop_server(Server *server);


#endif //SERVER_H
