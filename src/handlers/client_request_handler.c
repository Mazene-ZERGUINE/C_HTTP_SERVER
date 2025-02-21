//
// Created by Mazene ZERGUINZ on 21/02/2025.
//

#include "../../includes/client_request_handler.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

void handel_client_request(const int client_file_descriptor) {
    printf("Waiting for client request ...\n");

    printf("Client connected %d! \n", client_file_descriptor);
    const char *message = "Hello from server!\n";
    send(client_file_descriptor, message, strlen(message), 0);
    close(client_file_descriptor);
}
