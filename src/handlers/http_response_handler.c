#include <stdio.h>
#include <stdlib.h>
#include "../../includes/http_response_handler.h"

#include <unistd.h>

#include "logger.h"
#include "server.h"
#include "static_serving_handler.h"

#define CONTENT_LENGTH_BUFFER 20

char* size_t_to_string(size_t file_size) {
    char *size_str = malloc(CONTENT_LENGTH_BUFFER);
    if (!size_str) {
        log_error("Failed to allocate memory for Content-Length");
        return NULL;
    }
    snprintf(size_str, CONTENT_LENGTH_BUFFER, "%zu", file_size);
    return size_str;
}


HttpResponse* handel_http_response(const HttpRequest *http_request, const Server *server, size_t *response_length, char **response_body) {
    HttpResponse *http_response = http_response_new();

    if (strcmp(http_request->path, "/" ) == 0 || strcmp(http_request->path, "/index") == 0) {
        char *mime_type;
        size_t file_size;
        *response_body = serve_static_file("/index.html", server->app_config, &mime_type, &file_size);

        if (*response_body) {
            char size_str[20];
            snprintf(size_str, 20, "%zu", file_size);

            build_http_response(http_response, 200, NULL);
            add_http_response_header(http_response, "Content-Type", mime_type);
            add_http_response_header(http_response, "Content-Length", size_str);
            *response_length = file_size;

            free(mime_type);
            return http_response;
        } else {
            *response_body = strdup("<h1>Welcome to PyServe</h1><p>Your server is running successfully!</p>");
            *response_length = strlen(*response_body);
            build_http_response(http_response, 200, *response_body);
            add_http_response_header(http_response, "Content-Type", "text/html");

            return http_response;
        }
    }

    if (strncmp(http_request->path, "/public/", 8) == 0) {
        char *mime_type;
        size_t file_size;
        *response_body = serve_static_file(http_request->path, server->app_config, &mime_type, &file_size);

        if (*response_body) {
            char size_str[20];
            snprintf(size_str, 20, "%zu", file_size);

            build_http_response(http_response, 200, NULL);  // ✅ No inline body
            add_http_response_header(http_response, "Content-Type", mime_type);
            add_http_response_header(http_response, "Content-Length", size_str);
            *response_length = file_size;

            free(mime_type);
            return http_response;
        }
    }

    *response_body = strdup("<h1>404 - Not Found</h1>");
    *response_length = strlen(*response_body);
    build_http_response(http_response, 404, *response_body);
    add_http_response_header(http_response, "Content-Type", "text/html");

    return http_response;
}



void send_response(int file_descriptor, const char *headers, const char *body, size_t body_length) {
    ssize_t total_sent = send(file_descriptor, headers, strlen(headers), 0);
    if (total_sent < 0) {
        log_error("Failed to send HTTP headers");
        return;
    }
    if (body && body_length > 0) {
        size_t total_bytes_sent = 0;
        while (total_bytes_sent < body_length) {
            ssize_t bytes_sent = send(file_descriptor, body + total_bytes_sent, body_length - total_bytes_sent, 0);
            if (bytes_sent <= 0) {
                log_error("Failed to send complete binary response");
                break;
            }
            total_bytes_sent += bytes_sent;
        }
    }
    close(file_descriptor);
}
