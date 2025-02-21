#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "../includes/server.h"
#include "../src/core/config.h"
#include <string.h>

void run_server(const char *error_ptr, const char *port) {
    Server *server = create_server(port, MAX_CONNEXIONS);
    if (server == NULL) {
        perror(error_ptr);
        exit(EXIT_FAILURE);
    }

    start_server(server);
}


int main(int argc, const char *argv[]) {
     char *error_ptr = NULL;
    const char *port = "5400";

    if (argc > 1) {
        port = argv[1];
    }
    run_server(error_ptr, port);
    return EXIT_SUCCESS;
}

