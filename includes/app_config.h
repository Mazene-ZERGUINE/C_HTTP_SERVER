#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/**
 * @file app_config.h
 * @brief Defines the AppConfig structure and functions for managing the programme configurations.
 *
 * This file contains declarations for loading and handling application configuration settings,
 * including app name, resource paths, debug mode, and versioning.
 */

#include "logger.h"

/**
 * @struct AppConfig
 * @brief Stores application configuration settings.
 *
 * This structure holds key configuration parameters for the server, such as:
 * - The application name.
 * - The path to static resources.
 * - The application version.
 * - Debug mode status.
 */
typedef struct AppConfig {
    char *app_name;            /**< Name of the application */
    char *app_resources_path;  /**< Path to static resources (e.g., HTML, CSS, images) */
    int app_version;           /**< Version number of the application */
    int debug_mode;            /**< Debug mode flag (1 = enabled, 0 = disabled) */
} AppConfig;

/**
 * @brief Loads configuration settings from a configuration file.
 *
 * This function reads application settings from a predefined configuration file
 * and initializes the given AppConfig structure.
 *
 * @param app_config Pointer to the AppConfig structure to be populated.
 */
void load_config_from_file(AppConfig *app_config);

/**
 * @brief Initializes a new AppConfig structure with default values.
 *
 * Allocates memory for a new AppConfig structure and sets default values.
 *
 * @return Pointer to the newly initialized AppConfig structure.
 */
void *app_config_init(void);

/**
 * @brief Replaces placeholders in a given string with a specified replacement value.
 *
 * Scans the input content for a specified placeholder and replaces all occurrences
 * with the given replacement string. If no placeholders are found, a duplicate of
 * the original string is returned.
 *
 * @param content The original string containing placeholders.
 * @param placeholder The placeholder to be replaced (e.g., "%APP_NAME%").
 * @param replacement The string that replaces the placeholder.
 * @return A new string with placeholders replaced (must be freed by the caller).
 */
char *replace_placeholders(const char *content, const char *placeholder, const char *replacement);

#endif // APP_CONFIG_H
