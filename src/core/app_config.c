//
// Created by Mazene ZERGUINZ on 23/02/2025.
//

#include "../../includes/app_config.h"

#include "config.h"


void load_config_from_file(AppConfig *app_config) {
    FILE *config = fopen(CONFIG_FILE_PATH, "r");
    if (!config) {
        log_error("Failed to open config file", CONFIG_FILE_PATH);
        exit(EXIT_FAILURE);
    }
    char line[256];
    while (fgets(line, sizeof(line), config)) {
        if (strncmp(line, "app=", 4) == 0) {
            app_config->app_name = strdup(line + 4);
            app_config->app_name[strcspn(app_config->app_name, "\n")] = 0; // Remove newline
        }
        if (strncmp(line, "app_path=", 9) == 0) {
            app_config->app_resources_path = strdup(line + 9);
            app_config->app_resources_path[strcspn(app_config->app_resources_path, "\n")] = 0;
        }
        if (strncmp(line, "app_version=", 12) == 0) {
            app_config->app_version = atoi(line + 12);
        }
        if (strncmp(line, "enable_debug=", 13) == 0) {
            app_config->debug_mode = atoi(line + 13);
        }
    }
    fclose(config);
}

void *app_config_init(void) {
    AppConfig *app_config = (AppConfig *)malloc(sizeof(AppConfig));
    if (!app_config) {
        log_error("Failed to allocate memory for app_config", CONFIG_FILE_PATH);
        exit(EXIT_FAILURE);
    }

    load_config_from_file(app_config);
    return app_config;
}
