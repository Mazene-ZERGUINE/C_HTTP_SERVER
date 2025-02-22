#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "http_request_parser.h"
#include <string.h>

typedef struct {
    int status_code;
    const char *reason_phrase;
} HttpStatus;

static const HttpStatus HTTP_STATUS_LIST[] = {
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {204, "No Content"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {304, "Not Modified"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {503, "Service Unavailable"},
};

typedef struct {
    const char *http_version;
    int status_code;
    const char *reason_phrase;
    HttpHeader *headers;
    int header_count;
    char *body;
} HttpResponse;


HttpResponse* http_response_new(void);
void build_http_response(HttpResponse* http_response, int status_code, const char* body);
void add_http_response_header(HttpResponse* http_response, const char* key, const char* value);
char* http_response_to_string(HttpResponse* http_response);
void free_http_response(HttpResponse* response);

const char* get_http_reason_phrase(int status_code);
int get_http_status_code_from_phrase(const char* reason_phrase);


#endif // HTTP_RESPONSE_H
