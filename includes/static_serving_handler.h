#ifndef STATIC_SERVING_HANDLER_H
#define STATIC_SERVING_HANDLER_H

/**
 * @file static_serving_handler.h
 * @brief Handles static file serving and MIME type detection.
 *
 * This header file provides functions for serving static files from the server's resource path
 * and determining the correct MIME type for various file extensions.
 */

#include "app_config.h"

/**
 * @brief Retrieves the MIME type for a given file path.
 *
 * This function determines the appropriate MIME type based on the file extension.
 * It supports common file types such as HTML, CSS, JavaScript, images, and JSON.
 *
 * @param file_path The file path or name to determine the MIME type for.
 * @return A string representing the MIME type (e.g., "text/html", "image/png").
 */
const char *get_mime_type(const char *file_path);

/**
 * @brief Serves a static file by reading its contents into memory.
 *
 * This function loads a file from the server's static resource directory and returns its content.
 * It also determines the appropriate MIME type and file size.
 *
 * @param request_path The requested file path relative to the server's resource directory.
 * @param app_config Pointer to the AppConfig structure containing the resource directory path.
 * @param mime_type Pointer to store the dynamically allocated MIME type string (must be freed by the caller).
 * @param file_size Pointer to store the size of the file in bytes.
 * @return A dynamically allocated string containing the file's content (must be freed by the caller).
 *         Returns NULL if the file cannot be found or read.
 */
char *serve_static_file(const char *request_path, const AppConfig *app_config, char **mime_type, size_t *file_size);

#endif // STATIC_SERVING_HANDLER_H
