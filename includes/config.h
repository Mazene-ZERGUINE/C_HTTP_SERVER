#ifndef CONFIG_H
#define CONFIG_H

#define DEFAULT_PORT 5400
#define MAX_CONNEXIONS 10
#define DEFAULT_IP "127.0.0.1"
#define CONFIG_FILE_PATH get_config_file_path()
#define APPS_DIRECTORY get_apps_directory()
#define ASSETS_DIRECTORY get_assets_directory()

#include <stdlib.h>

static inline char* get_assets_directory() {
    const char *home = getenv("HOME");
    if (!home) {
        return "/home/Pyserve/assets/";
    }

    static char config_path[256];
    snprintf(config_path, sizeof(config_path), "%s/Pyserve/assets/", home);

    return config_path;
}

static inline char* get_config_file_path(void) {
    const char *home = getenv("HOME");
    if (!home) {
        return "/home/Pyserve/config/server.conf";
    }

    static char config_path[256];
    snprintf(config_path, sizeof(config_path), "%s/Pyserve/config/server.conf", home);

    return config_path;
}

static inline char* get_apps_directory(void) {
    const char *home = getenv("HOME");
    if (!home) {
        return "/home/Pyserve/apps/";
    }

    static char config_path[256];
    snprintf(config_path, sizeof(config_path), "%s/Pyserve/apps/", home);

    return config_path;
}

#endif //CONFIG_H
