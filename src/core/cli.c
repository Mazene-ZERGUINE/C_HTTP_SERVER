//
// Created by Mazene ZERGUINZ on 26/02/2025.
//

#include "../../includes/cli.h"

#include <unistd.h>
#include <sys/stat.h>

#include "config.h"
#include "logger.h"
#include "str_utils.h"

void set_assets(void) {

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void set_routing(const char *app_path) {
    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s/app.routes.json", app_path);

    FILE *file = fopen(file_path, "w");
    if (!file) {
        perror("Error while creating app.routes.json");
        return;
    }

    fprintf(file,
        "// add some documentation on how to use here\n"
        "[\n"
        "    {\n"
        "        \"path\": \"/home\",\n"
        "        \"view\": \"/index.html\",\n"
        "        \"children\": []\n"
        "    }\n"
        "]\n"
    );

    fclose(file);
    printf("✅ app.routes.json successfully created at %s\n", file_path);
}

void set_index(const char *app_path) {
    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s/index.html", app_path);

    FILE *file = fopen(file_path, "w");
    if (!file) {
        perror("Error while creating index.html");
        return;
    }

    fprintf(file,
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>Welcome to PyServe</title>\n"
        "    <link rel=\"icon\" type=\"image/png\" href=\"./public/favicon.ico\">\n"
        "    <link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css2?family=Inter:wght@400;600;700&display=swap\">\n"
        "    <style>\n"
        "        :root {\n"
        "            --primary-color: #61dafb;\n"
        "            --bg-color: #20232a;\n"
        "            --text-color: #ffffff;\n"
        "            --card-bg: #282c34;\n"
        "        }\n"
        "        body {\n"
        "            font-family: 'Inter', sans-serif;\n"
        "            text-align: center;\n"
        "            margin: 0;\n"
        "            padding: 0;\n"
        "            background-color: var(--bg-color);\n"
        "            color: var(--text-color);\n"
        "            display: flex;\n"
        "            justify-content: center;\n"
        "            align-items: center;\n"
        "            height: 100vh;\n"
        "        }\n"
        "        .container {\n"
        "            max-width: 1400px;\n"
        "            padding: 30px;\n"
        "            background: var(--card-bg);\n"
        "            border-radius: 12px;\n"
        "            box-shadow: 0 10px 20px rgba(0, 0, 0, 0.3);\n"
        "            text-align: center;\n"
        "        }\n"
        "        .logo {\n"
        "            width: 320px;\n"
        "        }\n"
        "        h1 {\n"
        "            color: var(--primary-color);\n"
        "            font-size: 32px;\n"
        "        }\n"
        "        p {\n"
        "            font-size: 18px;\n"
        "            opacity: 0.9;\n"
        "            margin-bottom: 20px;\n"
        "        }\n"
        "        .info {\n"
        "            background: rgba(255, 255, 255, 0.1);\n"
        "            padding: 15px;\n"
        "            border-radius: 8px;\n"
        "            margin: 20px 0;\n"
        "        }\n"
        "        .highlight {\n"
        "            color: var(--primary-color);\n"
        "            font-weight: bold;\n"
        "        }\n"
        "        .footer {\n"
        "            margin-top: 20px;\n"
        "            font-size: 14px;\n"
        "            opacity: 0.6;\n"
        "        }\n"
        "        a {\n"
        "            color: var(--primary-color);\n"
        "            text-decoration: none;\n"
        "            font-weight: bold;\n"
        "        }\n"
        "        a:hover {\n"
        "            text-decoration: underline;\n"
        "        }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <div class=\"container\">\n"
        "        <img src=\"./public/pyserve.png\" alt=\"PyServe Logo\" class=\"logo\">\n"
        "        <h1>Welcome to <span class=\"highlight\">PyServe</span></h1>\n"
        "        <div class=\"info\">\n"
        "            <p><strong>Server Name:</strong> PyServe</p>\n"
        "            <p><strong>Current Web App:</strong> <span class=\"highlight\">%APP_NAME%</span></p>\n"
        "            <p><strong>Serving Path:</strong> <span class=\"highlight\">%WEB_ROOT%</span></p>\n"
        "        </div>\n"
        "        <h2>🚀 Getting Started</h2>\n"
        "        <p>Edit your web files in:</p>\n"
        "        <p class=\"highlight\">~/Pyserve/apps/%APP_NAME%</p>\n"
        "        <h2>📄 Need Help?</h2>\n"
        "        <p>Check out the <a href=\"https://pyserve-docs.example.com\" target=\"_blank\">PyServe Documentation</a></p>\n"
        "        <div class=\"footer\">\n"
        "            <p>PyServe - The Lightweight Python HTTP Server | Created with ❤️</p>\n"
        "        </div>\n"
        "    </div>\n"
        "</body>\n"
        "</html>\n"
    );

    fclose(file);
    printf("✅ index.html successfully created at %s\n", file_path);
}

void init_app(const char *app_path) {
    if (!app_path) {
        printf("Error: Application path is NULL.\n");
        return;
    }

    set_routing(app_path);
    set_index(app_path);
}

void display_help(void) {
    printf("\033[1;32mConsole Tools\033[0m\n");
    printf("\033[1;33mUsage:\033[0m\n");
    printf(" Command [option] [argument] \n\n");

    printf("\033[1;33mAvailable commands:\033[0m\n");
    printf(" \033[1;32mcreate-app\033[0m [app_name]         creates a new application\n");
    printf(" \033[1;32mstart-app\033[0m [app_name]          runs the server and starts the selected application at port 5400 (default)\n");
    printf(" \033[1;32mlist-apps\033[0m                     lists all available applications\n");
    printf(" \033[1;32mdelete-app\033[0m [app_name]         deletes the selected application \n");
    printf(" \033[1;32mexit\033[0m                          exits the prompt\n");

    printf("\n");
    printf("\033[1;33mAvailable options:\033[0m\n");
    printf(" \033[1;32m--debug\033[0m                        runs the server in debug mode\n");
    printf(" \033[1;32m--no-debug\033[0m                     runs the server in no debug mode (default)\n");
    printf(" \033[1;32m--port\033[0m [port]                  runs the server on a different port (5400 by default)\n");
}

void display_welcome(void) {
    printf("\033%s", GREEN);

    printf("\n\n");
    printf(" \\______   \\__  |   |/   _____/\\_   _____/\\______   \\   \\ /   /\\_   _____/\n");
    printf("  |     ___//   |   |\\_____  \\  |    __)_  |       _/\\   Y   /  |    __)_ \n");
    printf("  |    |    \\____   |/        \\ |        \\ |    |   \\ \\     /   |        \\\n");
    printf("  |____|    / ______/_______  //_______  / |____|_  /  \\___/   /_______  /\n");
    printf("            \\/              \\/         \\/         \\/                   \\/ \n");

    printf("\n\033[1;36mWelcome to PyServe!\033[0m\n");
    printf("\033[1;33mType 'help' for more information.\033[0m\n\n");

    printf("\033[1;37m");
    printf("PyServe is a lightweight, high-performance HTTP server written in C.\n");
    printf("It allows you to run Python applications natively, serve static files, \n");
    printf("and define custom routes with hot reloading.\n\n");

    printf("✨ Features:\n");
    printf(" - 🚀 Runs Python applications directly without Flask or Django.\n");
    printf(" - 🔥 Supports dynamic routes and static file serving.\n");
    printf(" - ⚡ High-performance, lightweight, and cross-platform.\n");
    printf(" - 🔄 Hot reloading: No need to restart when routes change.\n\n");

    printf("💡 Start by typing '\033[1;32mhelp\033[1;37m' to explore available commands.\n\n");

    printf("\033[0m");
}


void create_app(const char *create_app_command) {
    int length = 0;
    char **tokens = str_split(create_app_command, " ", &length);
    if (!tokens || length != 2) {
        printf("\033[1;31m❌ Invalid command. Check 'help' for more information.\033[0m\n");
        printf("\033[1;33mUsage:\033[0m create-app MyApplication\n");
        return;
    }

    char app_path[512];
    snprintf(app_path, sizeof(app_path), "%s%s", APPS_DIRECTORY, tokens[1]);

    if (mkdir(app_path, 0777) == -1) {
        perror("\033[1;31m❌ Error while creating application root directory\033[0m");
        free_split(tokens);
        return;
    }

    printf("\033[1;32m✅ Creating %s Application...\033[0m\n", tokens[1]);

    const char *folders[] = {"/api", "/js", "/public", "/styles", "/templates"};
    for (int i = 0; i < 5; i++) {
        char subfolder_path[512];
        snprintf(subfolder_path, sizeof(subfolder_path), "%s%s", app_path, folders[i]);

        sleep(1);
        if (mkdir(subfolder_path, 0777) == -1) {
            perror("\033[1;31m❌ Error while creating application subdirectory\033[0m");
            free_split(tokens);
            return;
        }
        printf("\033[1;32m✅ Generated folder:\033[0m %s\n", subfolder_path);
    }

    const char *files[] = {"/index.html", "/index.js", "/app.routes.json"};
    for (int i = 0; i < 3; i++) {
        char file_path[512];
        snprintf(file_path, sizeof(file_path), "%s%s", app_path, files[i]);

        FILE *file = fopen(file_path, "w");
        if (!file) {
            perror("\033[1;31m❌ Error while creating application file\033[0m");
            free_split(tokens);
            return;
        }
        fclose(file);
        sleep(1);
        printf("\033[1;32m✅ Created file:\033[0m %s\n", file_path);
    }

    printf("\033[1;32m✅ Initializing application...\033[0m\n");
    init_app(app_path);

    printf("\033[1;32m🚀 Successfully created %s application! \033[0m\n", tokens[1]);
    printf("\n\n");

    printf("\033[1;33mNext: start-app %s to run the server\033[0m\n", tokens[1]);
    free_split(tokens);
}

void start_app(const char *start_app_command) {
    int length = 0;
    char** tokens = str_split(start_app_command, " ", &length);

    if (!tokens || length < 2) {
        perror("\033[1;31m❌ Can't start the server (missing app name) try help for more information\033[0m");
        return;
    }
    
}


