#include "../../includes/http_request_parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "str_utils.h"


HttpMethod get_http_method(const char *method_str) {
    if (strcmp(method_str, "GET") == 0) return HTTP_GET;
    if (strcmp(method_str, "POST") == 0) return HTTP_POST;
    if (strcmp(method_str, "PUT") == 0) return HTTP_PUT;
    if (strcmp(method_str, "DELETE") == 0) return HTTP_DELETE;
    if (strcmp(method_str, "HEAD") == 0) return HTTP_HEAD;
    if (strcmp(method_str, "OPTIONS") == 0) return HTTP_OPTIONS;
    if (strcmp(method_str, "PATCH") == 0) return HTTP_PATCH;
    return HTTP_UNKNOWN;
}

const char *http_method_to_string(HttpMethod method) {
    switch (method) {
        case HTTP_GET: return "GET";
        case HTTP_POST: return "POST";
        case HTTP_PUT: return "PUT";
        case HTTP_DELETE: return "DELETE";
        case HTTP_HEAD: return "HEAD";
        case HTTP_OPTIONS: return "OPTIONS";
        case HTTP_PATCH: return "PATCH";
        default: return "UNKNOWN";
    }
}

HttpRequest *parse_http_request(const char *request_str) {
    HttpRequest *http_request = malloc(sizeof(HttpRequest));
    if (http_request == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    char *req_copy = strdup(request_str);
    if (req_copy == NULL) {
        fprintf(stderr, "Failed to duplicate request string\n");
        free(http_request);
        return NULL;
    }

    // Extract request line and get pointer to remaining headers
    char *headers_start = parse_http_request_line(req_copy, http_request);
    if (!headers_start) {
        free(http_request);
        free(req_copy);
        return NULL;
    }

    if (parse_http_header(headers_start, http_request) == -1) {
        free(http_request);
        free(req_copy);
        return NULL;
    }

    free(req_copy);
    return http_request;
}


char *parse_http_request_line(const char *req_copy, HttpRequest *http_request) {
    const char *ptr = req_copy;

    char *method = extract_token(&ptr, " ");
    char *path = extract_token(&ptr, " ");
    char *version = extract_token(&ptr, "\n");

    if (!method || !path || !version) {
        fprintf(stderr, "Malformed HTTP request\n");
        free(method); free(path); free(version);
        return NULL;
    }

    http_request->http_method = get_http_method(method);
    http_request->path = path;
    http_request->http_version = version;
    free(method);

    char *headers_start = strstr(req_copy, "\r\n");
    if (!headers_start) {
        fprintf(stderr, "No headers found\n");
        return NULL;
    }

    return headers_start + 2;
}


int parse_http_header(char *headers_start, HttpRequest *http_request) {
    if (!headers_start) {
        fprintf(stderr, "No headers to parse\n");
        return -1;
    }

    http_request->header_count = 0;
    int capacity = INITIAL_HEADER_CAPACITY;
    http_request->headers = malloc(sizeof(HttpHeader) * capacity);
    if (!http_request->headers) {
        fprintf(stderr, "Memory allocation error for headers\n");
        return -1;
    }

    char *line = strtok(headers_start, "\r\n");
    while (line != NULL && strlen(line) > 0) {
        char *sep = strchr(line, ':');
        if (!sep) {
            line = strtok(NULL, "\r\n");
            continue;
        }

        *sep = '\0';
        char *key = line;
        char *value = sep + 1;

        while (*key == ' ') key++;
        char *key_end = key + strlen(key) - 1;
        while (key_end > key && *key_end == ' ') *key_end-- = '\0';

        while (*value == ' ') value++;
        char *value_end = value + strlen(value) - 1;
        while (value_end > value && *value_end == ' ') *value_end-- = '\0';

        if (http_request->header_count >= capacity) {
            capacity *= 2;
            HttpHeader *new_headers = realloc(http_request->headers, sizeof(HttpHeader) * capacity);
            if (!new_headers) {
                fprintf(stderr, "Memory allocation failed for headers\n");
                free(http_request->headers);
                return -1;
            }
            http_request->headers = new_headers;
        }

        http_request->headers[http_request->header_count].key = strdup(key);
        http_request->headers[http_request->header_count].value = strdup(value);
        http_request->header_count++;

        line = strtok(NULL, "\r\n");
    }

    return 0;
}


void free_http_request(HttpRequest *request) {
    if (!request) return;

    free(request->path);
    free(request->http_version);

    for (int i = 0; i < request->header_count; i++) {
        free(request->headers[i].key);
        free(request->headers[i].value);
    }
    free(request->headers);
    free(request);
}
