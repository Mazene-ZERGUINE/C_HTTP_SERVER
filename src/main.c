#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "../includes/server.h"
#include "../includes/config.h"
#include <string.h>
#include "cli.h"
#include "str_utils.h"

void run_server(const char *error_ptr, const char *port) {
    Server *server = create_server(port, MAX_CONNEXIONS);
    if (server == NULL) {
        perror(error_ptr);
        exit(EXIT_FAILURE);
    }

    start_server(server);
}


int main(void) {
    display_welcome();
    while (true) {
        char command[60];
        log_cli_prompt();
        fflush(stdin);
        read_line(command, 60);

        if (strcmp(command, "exit") == 0) exit(EXIT_SUCCESS);

        if (strcmp(command, "help") == 0) {
            display_help();
        }

        else if (strstr(command, "create-app")) {
            create_app(command);
        }

        else if (strstr(command, "start-app")) {
            printf("Start App\n");
            const char *error_ptr = NULL;
            const char *port = "5400";
            run_server(error_ptr, port);
            return EXIT_SUCCESS;
        }

        else if (strstr(command, "delete-app")) {
            printf("Delete App\n");
        }

        else if (strstr(command, "list-apps")) {
            printf("List Apps\n");
        } else {
            printf("Unknown command try 'help' for more informations \n");
        }
    }
}

