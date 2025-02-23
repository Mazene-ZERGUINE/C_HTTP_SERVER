#include "static_serving_handler.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 8192

const char *get_mime_type(const char *file_path) {
    if (strstr(file_path, ".html")) return "text/html";
    if (strstr(file_path, ".css")) return "text/css";
    if (strstr(file_path, ".js")) return "application/javascript";
    if (strstr(file_path, ".png")) return "image/png";
    if (strstr(file_path, ".jpg") || strstr(file_path, ".jpeg")) return "image/jpeg";
    if (strstr(file_path, ".gif")) return "image/gif";
    if (strstr(file_path, ".svg")) return "image/svg+xml";
    if (strstr(file_path, ".ico")) return "image/x-icon";
    if (strstr(file_path, ".json")) return "application/json";
    return "application/octet-stream";  // Default unknown file type
}

char *serve_static_file(const char *request_path, const AppConfig *app_config, char **mime_type, size_t *file_size) {
    static char full_path[512];

    snprintf(full_path, sizeof(full_path), "%s%s", app_config->app_resources_path, request_path);
    log_debug("Serving static file: %s", full_path);

    FILE *file = fopen(full_path, "rb");
    if (!file) {
        log_error("Static file not found: %s", full_path);
        return NULL;
    }

    *mime_type = strdup(get_mime_type(full_path));

    // Ensure the file size is determined correctly
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);

    if (*file_size <= 0) {
        log_error("File size error: %zu bytes", *file_size);
        fclose(file);
        return NULL;
    }

    rewind(file);

    char *content = malloc(*file_size);
    if (!content) {
        log_error("Memory allocation failed for file content");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(content, 1, *file_size, file);
    fclose(file);

    if (bytes_read != *file_size) {
        log_error("File read error: Expected %zu bytes, but read %zu bytes", *file_size, bytes_read);
        free(content);
        return NULL;
    }

    return content;
}
