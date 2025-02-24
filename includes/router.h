/**
 * @file router.h
 * @brief Defines structures and functions for managing application routes.
 *
 * This module is responsible for parsing, loading, and resolving routes
 * from a JSON configuration file (`app.routes.json`). It supports nested
 * routes and dynamic updates through hot reloading.
 */

#ifndef ROUTER_H
#define ROUTER_H

#include <cjson/cJSON.h>
#include "logger.h"
#include "server.h"

#define INITIAL_ROUTES_SIZE 100  /**< Maximum number of routes supported */

/**
 * @struct Route
 * @brief Represents a single route in the routing system.
 *
 * Each route consists of a `path` (URL pattern) and a `view`
 * (file path to be served). Routes can have child routes, enabling
 * hierarchical navigation.
 */
typedef struct Route {
    char* path;             /**< The URL path of the route (e.g., "/home"). */
    char* view;             /**< The file path that serves the route (e.g., "templates/home.html"). */
    struct Route **children; /**< An array of child routes (nested routes). */
    int child_count;        /**< Number of child routes. */
} Route;

/**
 * @brief Holds all loaded routes in memory.
 *
 * This array stores pointers to dynamically allocated `Route` structures.
 * It is populated when `load_routes()` is called and used in `resolve_route()`
 * to match incoming requests.
 */
static Route **routes;

/**
 * @brief Keeps track of the number of loaded routes.
 *
 * This counter increments as new routes are parsed and stored in `routes`.
 */
static int routes_count;

/**
 * @brief Parses the given JSON string and loads routes into memory.
 *
 * This function extracts routes from the JSON configuration, ensuring
 * valid paths and views are set. It supports nested child routes.
 *
 * @param json_str The JSON string containing the route definitions.
 */
void parse_routes(const char *json_str);

/**
 * @brief Loads routes from the `app.routes.json` file.
 *
 * Reads the route configuration file, extracts routes, and stores them
 * in memory. Routes are dynamically allocated and stored in the `routes` array.
 *
 * @param server The server configuration containing the `app_resources_path`.
 */
void load_routes(const Server* server);

/**
 * @brief Resolves a request path to its corresponding view file.
 *
 * This function searches the loaded routes to find a match for the given request path.
 * If found, it returns the associated view file path; otherwise, it returns `NULL`.
 *
 * @param request_path The incoming request path (e.g., "/home/dashboard").
 * @return The file path of the corresponding view or `NULL` if not found.
 */
char *resolve_route(const char *request_path);

#endif // ROUTER_H
