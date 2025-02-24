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
char *replace_placeholders(const char *content, const char *placeholder, const char *replacement) {
    if (!content || !placeholder || !replacement) return NULL;

    const size_t content_len = strlen(content);
    const size_t placeholder_len = strlen(placeholder);
    size_t replacement_len = strlen(replacement);

    size_t count = 0;
    const char *pos = content;

    while ((pos = strstr(pos, placeholder)) != NULL) {
        count++;
        pos += placeholder_len;
    }

    if (count == 0) return strdup(content);

    const size_t new_size = content_len + (replacement_len - placeholder_len) * count + 1;
    char *new_content = malloc(new_size);
    if (!new_content) {
        log_error("Memory allocation failed for placeholder replacement");
        return NULL;
    }

    char *dest = new_content;
    pos = content;
    while (*pos) {
        const char *match = strstr(pos, placeholder);
        if (match) {
            size_t segment_len = match - pos;
            memcpy(dest, pos, segment_len);
            dest += segment_len;
            memcpy(dest, replacement, replacement_len);
            dest += replacement_len;
            pos = match + placeholder_len;
        } else {
            strcpy(dest, pos);
            break;
        }
    }

    return new_content;
}
