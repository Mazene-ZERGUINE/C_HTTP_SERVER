//
// Created by Mazene ZERGUINZ on 21/02/2025.
//

#include "../../includes/http_request_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    char *req_copy = strdup(request_str);
    if (req_copy == NULL) {
        fprintf(stderr, "Failed to duplicate request string\n");
        free(http_request);
        return NULL;
    }

    if (parse_http_request_line(req_copy, http_request) == -1) {
        free(http_request);
        free(req_copy);
        return NULL;
    }

    if (parse_http_header(req_copy, http_request) == -1) {
        free(http_request);
        free(req_copy);
        return NULL;
    }


    free(req_copy);
    return http_request;
}


int parse_http_request_line(char *req_copy, HttpRequest *http_request) {
    char **request_metadata = malloc(sizeof(char *) * 3);
    if (request_metadata == NULL) {
        fprintf(stderr, "Memory allocation error for metadata\n");
        return -1;
    }

    char *token = strtok(req_copy, " ");
    for (int i = 0; i < 3; i++) {
        if (token == NULL) {
            request_metadata[i] = NULL;
        } else {
            request_metadata[i] = strdup(token);
            token = strtok(NULL, " ");
        }
    }

    if (request_metadata[0] && request_metadata[1] && request_metadata[2]) {
        http_request->http_method = get_http_method(request_metadata[0]);
        http_request->path = strdup(request_metadata[1]);
        http_request->http_version = strdup(request_metadata[2]);
    } else {
        fprintf(stderr, "Malformed HTTP request\n");
        for (int i = 0; i < 3; i++) {
            if (request_metadata[i]) free(request_metadata[i]);
        }
        free(request_metadata);
        return -1;
    }

    for (int i = 0; i < 3; i++) {
        free(request_metadata[i]);
    }
    free(request_metadata);

    return 0;
}

int parse_http_header(char *req_copy, HttpRequest *http_request) {
    char *line = strtok(req_copy, "\r\n");
    if (line == NULL) {
        fprintf(stderr, "Error: Malformed HTTP request (missing request line)\n");
        return -1;
    }

    http_request->header_count = 0;
    http_request->headers = malloc(sizeof(HttpHeader) * 10);
    if (http_request->headers == NULL) {
        fprintf(stderr, "Memory allocation error for headers\n");
        return -1 ;
    }

    line = strtok(NULL, "\r\n");
    while (line && strlen(line) > 0) {
        char *key = strtok(line, ": ");
        char *value = strtok(NULL, "");

        if (key && value) {
            http_request->headers[http_request->header_count].key = strdup(key);
            http_request->headers[http_request->header_count].value = strdup(value);
            http_request->header_count++;
        }
        line = strtok(NULL, "\r\n");
    }
    return 0;
}
