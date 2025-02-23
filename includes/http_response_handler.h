#ifndef HTTP_RESPONSE_HANDLER_H
#define HTTP_RESPONSE_HANDLER_H

#include "http-response.h"
#include "server.h"

HttpResponse* handel_http_response(const HttpRequest *http_request, const Server *server, size_t *response_length, char **response_body);
void send_response(int file_descriptor, const char *headers, const char *body, size_t body_length);

#endif //HTTP_RESPONSE_HANDLER_H
