#include "../../includes/logger.h"

void log_message(const char *color, const char *level, const char *format, va_list args) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    printf("%s[%s] [%s]: ", color, time_str, level);
    vprintf(format, args);
    printf("%s\n", RESET);
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(RED, "ERROR", format, args);
    va_end(args);
}

void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(GREEN, "INFO", format, args);
    va_end(args);
}

void log_debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(CYAN, "DEBUG", format, args);
    va_end(args);
}

void log_warn(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(YELLOW, "WARN", format, args);
    va_end(args);
}

void log_request(const HttpRequest *http_request) {
    if (http_request == NULL) {
        log_error("Received NULL HttpRequest in log_request()");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    printf("%s[%s] [REQUEST]: Method: %s, Path: %s, HTTP Version: %s\n",
           BLUE, time_str,
           http_method_to_string(http_request->http_method),
           http_request->path,
           http_request->http_version);

    printf("Headers (%d):\n", http_request->header_count);
    for (int i = 0; i < http_request->header_count; i++) {
        printf("   %s: %s\n", http_request->headers[i].key, http_request->headers[i].value);
    }
    printf("%s\n", RESET);
}

void log_response(const HttpResponse *http_response) {
    if (http_response == NULL) {
        log_error("Received NULL HttpResponse in log_response()");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    const char *color;
    if (http_response->status_code >= 200 && http_response->status_code < 300) {
        color = GREEN;
    } else if (http_response->status_code >= 400 && http_response->status_code < 500) {
        color = YELLOW;
    } else if (http_response->status_code >= 500) {
        color = RED;
    } else {
        color = MAGENTA;
    }

    printf("%s[%s] [RESPONSE]: HTTP Version: %s, Status: %d %s\n",
           color, time_str,
           http_response->http_version,
           http_response->status_code,
           http_response->reason_phrase);

    printf("   Headers (%d):\n", http_response->header_count);
    for (int i = 0; i < http_response->header_count; i++) {
        printf("       %s: %s\n", http_response->headers[i].key, http_response->headers[i].value);
    }

    if (http_response->body) {
        printf("   Body: %.100s%s\n", http_response->body, strlen(http_response->body) > 100 ? "..." : "");
    }

    printf("%s\n", RESET); // Reset color
}

void log_cli_prompt(void) {
    printf("\033[1;35mPyServe ⇒ \033[0m");
}