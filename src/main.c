#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "../includes/server.h"
#include "../includes/config.h"
#include <string.h>
#include "cli.h"
#include "str_utils.h"


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
            start_app(command);
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

