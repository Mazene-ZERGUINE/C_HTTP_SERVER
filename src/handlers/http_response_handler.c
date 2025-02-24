#include <stdio.h>
#include <stdlib.h>
#include "../../includes/http_response_handler.h"

#include <unistd.h>

#include "logger.h"
#include "router.h"
#include "server.h"
#include "static_serving_handler.h"
#include "str_utils.h"



HttpResponse* handel_http_response(const HttpRequest *http_request, const Server *server, size_t *response_length, char **response_body) {
    HttpResponse *http_response = http_response_new();
    char *mime_type;
    size_t file_size;

    // 1️⃣ Try to resolve the request path using the routing system
    char *file_path = resolve_route(http_request->path);
    log_debug("http_response", "file_path: %s", file_path);

    // 2️⃣ If no custom route is found, fall back to default file resolution
    if (!file_path) {
        file_path = (strcmp(http_request->path, "/") == 0) ? "/index.html" : http_request->path;
    }

    // 3️⃣ Fetch the requested resource file
    *response_body = serve_static_file(file_path, server->app_config, &mime_type, &file_size);

    if (*response_body) {
        // 4️⃣ Calculate Content-Length
        char size_str[CONTENT_LENGTH_BUFFER];
        snprintf(size_str, CONTENT_LENGTH_BUFFER, "%zu", file_size);

        // 5️⃣ Modify the index.html content if needed
        if (strcmp(file_path, "/index.html") == 0) {
            char *modified_body = replace_placeholders(*response_body, "%APP_NAME%", server->app_config->app_name);
            char *final_body = replace_placeholders(modified_body, "%WEB_ROOT%", server->app_config->app_resources_path);
            free(modified_body);
            free(*response_body);
            *response_body = final_body;
            *response_length = strlen(final_body);
        } else {
            *response_length = file_size;
        }

        // 6️⃣ Build the HTTP response
        build_http_response(http_response, 200, NULL);
        add_http_response_header(http_response, "Content-Type", mime_type);
        add_http_response_header(http_response, "Content-Length", size_str);
        free(mime_type);
    } else {
        // 7️⃣ File not found, return 404 response
        *response_body = strdup("<h1>404 Not Found</h1><p>The requested resource was not found.</p>");
        *response_length = strlen(*response_body);
        build_http_response(http_response, 404, *response_body);
        add_http_response_header(http_response, "Content-Type", "text/html");
    }

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
