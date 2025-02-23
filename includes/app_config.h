//
// Created by Mazene ZERGUINZ on 23/02/2025.
//

#ifndef APP_CONFIG_H
#define APP_CONFIG_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

typedef struct  AppConfig {
    char *app_name;
    char* app_resources_path;
    int app_version;
    int debug_mode;
} AppConfig;

void load_config_from_file(AppConfig *app_config);
void *app_config_init(void);

#endif //APP_CONFIG_H
