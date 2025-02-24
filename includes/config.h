#ifndef CONFIG_H
#define CONFIG_H

#define DEFAULT_PORT 5400
#define MAX_CONNEXIONS 10
#define DEFAULT_IP "127.0.0.1"
#define CONFIG_FILE_PATH get_config_file_path()

static inline char* get_config_file_path(void) {
    const char *home = getenv("HOME");
    if (!home) {
        return "/home/Pyserve/server.conf";
    }

    static char config_path[256];
    snprintf(config_path, sizeof(config_path), "%s/Pyserve/config/server.conf", home);

    return config_path;
}

#endif //CONFIG_H
