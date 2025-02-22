//
// Created by Mazene ZERGUINZ on 22/02/2025.
//

#include "../../includes/http-response.h"

#include <stdio.h>

HttpResponse* http_response_new(void) {
    HttpResponse* response = (HttpResponse*)malloc(sizeof(HttpResponse));
    if (response == NULL) {
        fprintf(stderr, "Error allocating memory for HttpResponse\n");
        return NULL;
    }

    response->http_version = "HTTP/1.1";
    response->status_code = 200;
    response->reason_phrase = "OK";
    response->headers = NULL;
    response->header_count = 0;
    response->body = NULL;
    return response;
}

void build_http_response(HttpResponse* http_response, int status_code, const char* body) {
    for (size_t i = 0; i < sizeof(HTTP_STATUS_LIST) / sizeof(HttpStatus); i++) {
        if (HTTP_STATUS_LIST[i].status_code == status_code) {
            http_response->status_code = status_code;
            http_response->reason_phrase = HTTP_STATUS_LIST[i].reason_phrase;
            break;
        }
    }
    if (body) {
        http_response->body = strdup(body);
    }
}


void add_http_response_header(HttpResponse* http_response, const char* key, const char* value) {
    http_response->headers = realloc(http_response->headers, sizeof(HttpHeader) * (http_response->header_count + 1));
    if (!http_response->headers) {
        perror("Failed to allocate memory for headers");
        return;
    }

    http_response->headers[http_response->header_count].key = strdup(key);
    http_response->headers[http_response->header_count].value = strdup(value);
    http_response->header_count++;
}

char* http_response_to_string(HttpResponse* http_response) {
    size_t buffer_size = 1024 + (http_response->body ? strlen(http_response->body) : 0);
    char* response_str = malloc(buffer_size);

    snprintf(response_str, buffer_size, "%s %d %s\r\n",
             http_response->http_version,
             http_response->status_code,
             http_response->reason_phrase);

    for (int i = 0; i < http_response->header_count; i++) {
        snprintf(response_str + strlen(response_str), buffer_size - strlen(response_str),
                 "%s: %s\r\n", http_response->headers[i].key, http_response->headers[i].value);
    }

    strcat(response_str, "\r\n");
    if (http_response->body) {
        strcat(response_str, http_response->body);
    }

    return response_str;
}

void free_http_response(HttpResponse* response) {
    if (!response) return;

    free(response->body);

    for (int i = 0; i < response->header_count; i++) {
        free(response->headers[i].key);
        free(response->headers[i].value);
    }

    free(response->headers);
    free(response);
}


const char* get_http_reason_phrase(int status_code) {
    size_t status_count = sizeof(HTTP_STATUS_LIST) / sizeof(HttpStatus);
    for (size_t i = 0; i < status_count; i++) {
        if (HTTP_STATUS_LIST[i].status_code == status_code) {
            return HTTP_STATUS_LIST[i].reason_phrase;
        }
    }
    return "Unknown Status Code";
}

int get_http_status_code_from_phrase(const char* reason_phrase) {
    if (!reason_phrase) return -1; // Invalid input

    size_t status_count = sizeof(HTTP_STATUS_LIST) / sizeof(HttpStatus);
    for (size_t i = 0; i < status_count; i++) {
        if (strcmp(HTTP_STATUS_LIST[i].reason_phrase, reason_phrase) == 0) {
            return HTTP_STATUS_LIST[i].status_code;
        }
    }
    return -1; // Unknown phrase
}
