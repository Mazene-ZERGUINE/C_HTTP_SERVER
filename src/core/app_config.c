//
// Created by Mazene ZERGUINZ on 23/02/2025.
//

#include "../../includes/app_config.h"

#include "../../includes/config.h"


void load_config_from_file(AppConfig *app_config) {
    // loads and caches the currently running application from the server config file to reduce configuration file read operations
    FILE *config = fopen(CONFIG_FILE_PATH, "r");
    if (!config) {
        log_error("Failed to open config file", CONFIG_FILE_PATH);
        exit(EXIT_FAILURE);
    }
    char line[256];
    while (fgets(line, sizeof(line), config)) {
        if (strncmp(line, "app=", 4) == 0) {
            app_config->app_name = strdup(line + 4);
            app_config->app_name[strcspn(app_config->app_name, "\n")] = 0;
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

// replace the display placeholders in the default index.html page
char* replace_placeholders(const char *input, const char *placeholder, const char *replacement) {
    if (!input || !placeholder || !replacement) return NULL;

    size_t input_len = strlen(input);
    size_t placeholder_len = strlen(placeholder);
    size_t replacement_len = strlen(replacement);

    int count = 0;
    const char *tmp = input;
    while ((tmp = strstr(tmp, placeholder)) != NULL) {
        count++;
        tmp += placeholder_len;
    }

    if (count == 0) return strdup(input);

    size_t new_size = input_len + (replacement_len - placeholder_len) * count + 1;
    char *new_str = malloc(new_size);
    if (!new_str) return NULL;

    const char *src = input;
    char *dest = new_str;
    while ((tmp = strstr(src, placeholder)) != NULL) {
        // Copy content before the placeholder
        size_t prefix_len = tmp - src;
        memcpy(dest, src, prefix_len);
        dest += prefix_len;

        memcpy(dest, replacement, replacement_len);
        dest += replacement_len;

        src = tmp + placeholder_len;
    }
    strcpy(dest, src);

    return new_str;
}
