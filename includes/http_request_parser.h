//
// Created by Mazene ZERGUINZ on 21/02/2025.
//

#ifndef HTTP_REQUEST_PARSER_H
#define HTTP_REQUEST_PARSER_H

// Headers
typedef struct {
    char *key;
    char *value;
} HttpHeader;

// Request Methods
typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_PATCH,
    HTTP_UNKNOWN
} HttpMethod;

// HTTP request structure
typedef struct {
    HttpHeader *headers;
    HttpMethod http_method;
    char *path;
    char *http_version;
    char *host;
    char *agent;
    int header_count;
} HttpRequest;

HttpMethod get_http_method(const char *method);
const char *http_method_to_string(HttpMethod method);

HttpRequest *parse_http_request(const char *req_string);
int parse_http_request_line(char * req_copy, HttpRequest * http_request);
int parse_http_header( char *req_string, HttpRequest *http_header);


#endif //HTTP_REQUEST_PARSER_H
