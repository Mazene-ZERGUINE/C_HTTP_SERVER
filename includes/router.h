/**
 * @file router.h
 * @brief Defines structures and functions for managing application routes.
 *
 * This module is responsible for parsing, loading, and resolving routes
 * from a JSON configuration file (`app.routes.json`). It supports nested
 * routes and dynamic updates through hot reloading. The router system
 * is designed to handle URL path mappings to view files efficiently.
 */

#ifndef ROUTER_H
#define ROUTER_H

#include <cjson/cJSON.h>
#include "logger.h"
#include "server.h"

#define INITIAL_ROUTES_SIZE 100  /**< Maximum number of top-level routes supported. */

/**
 * @struct Route
 * @brief Represents a single route in the routing system.
 *
 * Each route consists of a `path` (URL pattern) and a `view`
 * (file path to be served). Routes can have nested child routes, enabling
 * hierarchical navigation. Child routes are stored as an array of pointers
 * to `Route` structures.
 */
typedef struct Route {
    char* path;              /**< The URL path of the route (e.g., "/home"). */
    char* view;              /**< The file path that serves the route (e.g., "templates/home.html"). */
    struct Route **children; /**< An array of pointers to child routes (nested routes). */
    int child_count;         /**< Number of child routes. */
} Route;

/**
 * @var routes
 * @brief Holds all loaded routes in memory.
 *
 * This global array stores pointers to dynamically allocated `Route` structures.
 * It is populated when `load_routes()` is called and is used in `resolve_route()`
 * to match incoming requests to their corresponding views.
 */
static Route **routes;

/**
 * @var routes_count
 * @brief Keeps track of the number of loaded routes.
 *
 * This counter is incremented as new routes are parsed and stored in `routes`.
 * It is used to limit route resolution and ensure efficient processing.
 */
static int routes_count;

/**
 * @brief Loads and parses the application routes from `app.routes.json`.
 *
 * This function reads the JSON configuration file containing route definitions
 * and dynamically constructs a tree of `Route` structures, allowing the
 * HTTP server to resolve incoming requests.
 *
 * @param server A pointer to the `Server` structure containing application configuration details.
 */
void load_routes(const Server *server);

/**
 * @brief Recursively parses a route JSON object and constructs a Route structure.
 *
 * This function processes both top-level and nested child routes, ensuring that
 * deep hierarchical routing is supported. It constructs each `Route` instance
 * with the correct path and view mappings.
 *
 * @param route_json A pointer to a `cJSON` object representing the route.
 * @param parent_path The parent path of the current route (used for building full paths).
 * @return A pointer to a dynamically allocated `Route` structure, or NULL if an error occurs.
 */
Route *parse_route_recursive(cJSON *route_json, const char *parent_path);

/**
 * @brief Parses the entire routes JSON structure into memory.
 *
 * This function iterates through the JSON file, extracting routes and
 * storing them in a globally accessible `routes` array. It calls
 * `parse_route_recursive()` to handle nested routes.
 *
 * @param json_str A string containing the JSON representation of the routes.
 */
void parse_routes(const char *json_str);

/**
 * @brief Recursively resolves a requested path within a route structure.
 *
 * This function performs a depth-first search to find a matching route
 * within the given `Route` structure. It checks both the current route
 * and all of its children.
 *
 * @param route A pointer to the `Route` structure being searched.
 * @param request_path The requested URL path.
 * @return A dynamically allocated string representing the associated view file, or NULL if not found.
 */
char *resolve_route_recursive(Route *route, const char *request_path);

/**
 * @brief Resolves a route by matching the request path.
 *
 * This function searches the global `routes` array to find a route
 * that matches the requested path. If a match is found, it returns
 * the corresponding view file path.
 *
 * @param request_path The requested URL path.
 * @return A dynamically allocated string representing the associated view file, or NULL if not found.
 */
char *resolve_route(const char *request_path);

#endif // ROUTER_H
