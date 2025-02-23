#include "../../includes/http_response_handler.h"
#include <sys/socket.h>
#include "logger.h"

char* handel_http_response(const HttpRequest *http_request) {

    HttpResponse *http_response = http_response_new();
    if (strcmp(http_request->path, "/") == 0) {
        build_http_response(http_response, 200, "<h1>Welcome to the Server!</h1>");
        add_http_response_header(http_response, "Content-Type", "text/html");
    } else {
        build_http_response(http_response, 404, "<h1>404 - Not Found</h1>");
        add_http_response_header(http_response, "Content-Type", "text/html");
    }

    char* response_string = http_response_to_string(http_response);

    log_response(http_response);

    free_http_response(http_response);
    return response_string;
}

void send_response(int file_descriptor, char* response_string) {
    send(file_descriptor, response_string ,strlen(response_string)  ,0);
}
