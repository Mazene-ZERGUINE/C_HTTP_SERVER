#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "http-response.h"
#include "http_request_parser.h"

/**
 * @enum LogLevel
 * @brief Defines different levels of logging.
 */
typedef enum LogLevel {
    LOG_INFO,   ///< Informational messages (e.g., server started)
    LOG_DEBUG,  ///< Debugging messages (e.g., request received)
    LOG_WARN,   ///< Warnings (e.g., high memory usage)
    LOG_ERROR   ///< Errors (e.g., failed request handling)
} LogLevel;

/**
 * ANSI escape codes for colored terminal output.
 * Used to distinguish log messages by severity.
 */
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

/**
 * @brief Logs a formatted message with a specified color and log level.
 *
 * @param color The ANSI color code for the log message.
 * @param level The log level (INFO, DEBUG, WARN, ERROR).
 * @param format The format string (similar to printf).
 * @param args The variadic arguments list.
 */
void log_message(const char *color, const char *level, const char *format, va_list args);

/**
 * @brief Logs an error message.
 *
 * @param format The format string for the log message.
 * @param ... Variadic arguments for formatting.
 */
void log_error(const char *format, ...);

/**
 * @brief Logs an informational message.
 *
 * @param format The format string for the log message.
 * @param ... Variadic arguments for formatting.
 */
void log_info(const char *format, ...);

/**
 * @brief Logs a debug message.
 *
 * @param format The format string for the log message.
 * @param ... Variadic arguments for formatting.
 */
void log_debug(const char *format, ...);

/**
 * @brief Logs a warning message.
 *
 * @param format The format string for the log message.
 * @param ... Variadic arguments for formatting.
 */
void log_warn(const char *format, ...);

/**
 * @brief Logs an HTTP request.
 *
 * @param http_request Pointer to an `HttpRequest` struct containing request details.
 */
void log_request(const HttpRequest *http_request);

/**
 * @brief Logs an HTTP response.
 *
 * @param http_response Pointer to an `HttpResponse` struct containing response details.
 */
void log_response(const HttpResponse *http_response);

#endif // LOGGER_H
