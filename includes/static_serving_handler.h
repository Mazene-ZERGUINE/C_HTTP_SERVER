
#ifndef STATIC_SERVING_HANDLER_H
#define STATIC_SERVING_HANDLER_H
#include "app_config.h"

const char *get_mime_type(const char *file_path);
char *serve_static_file(const char *request_path, const AppConfig *app_config, char **mime_type, size_t *file_size);
#endif //STATIC_SERVING_HANDLER_H
