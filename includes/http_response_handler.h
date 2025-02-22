#ifndef HTTP_RESPONSE_HANDLER_H
#define HTTP_RESPONSE_HANDLER_H

#include "http-response.h"

char* handel_http_response(const HttpRequest *http_request);
void send_response(int file_descriptor, char* response_string);

#endif //HTTP_RESPONSE_HANDLER_H
