#include "../../includes/router.h"
#include "server.h"
#include <sys/stat.h>

void load_routes(const Server *server) {
    static time_t last_modified = 0;
    struct stat file_stat;

    char *routes_file_path = strdup(server->app_config->app_resources_path);
    strcat(routes_file_path, "/app.routes.json");

    // checking latest modifications on the routing file (app.routes.json)
    if (stat(routes_file_path, &file_stat) == 0) {
        // if no new modification detected exit the function
        if (file_stat.st_mtime <= last_modified) {
            free(routes_file_path);
            return;
        }
        last_modified = file_stat.st_mtime;
    } else {
        log_error("Failed to access app.routes.json. Server shutdown...");
        free(routes_file_path);
        exit(EXIT_FAILURE);
    }
    log_info("Reloading routes from file...");

    // reading the file and updating the routes only in case of new modifications have been made
    FILE *file = fopen(routes_file_path, "r");
    if (!file) {
        log_error("app.routes.json not found. Server shutdown...");
        free(routes_file_path);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);
    rewind(file);

    char *json_str = malloc(file_size + 1);
    fread(json_str, 1, file_size, file);
    json_str[file_size] = '\0';
    fclose(file);
    free(routes_file_path);

    parse_routes(json_str);
    free(json_str);
}


Route *parse_route_recursive(cJSON *route_json, const char *parent_path) {
    cJSON *path_item = cJSON_GetObjectItem(route_json, "path");
    cJSON *view_item = cJSON_GetObjectItem(route_json, "view");

    if (!path_item || !cJSON_IsString(path_item)) {
        log_error("Malformed routes.json: Missing or invalid 'path'.");
        return NULL;
    }
    if (!view_item || !cJSON_IsString(view_item)) {
        log_error("Malformed routes.json: Missing or invalid 'view'.");
        return NULL;
    }

    Route *new_route = (Route *)malloc(sizeof(Route));
    if (!new_route) {
        log_error("Memory allocation failed for new route.");
        return NULL;
    }

    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s%s", parent_path, path_item->valuestring);
    new_route->path = strdup(full_path);
    new_route->view = strdup(view_item->valuestring);
    new_route->child_count = 0;
    new_route->children = NULL;

    cJSON *children = cJSON_GetObjectItem(route_json, "children");
    if (children && cJSON_IsArray(children)) {
        int child_count = cJSON_GetArraySize(children);
        new_route->children = malloc(child_count * sizeof(Route *));
        if (!new_route->children) {
            log_error("Memory allocation failed for child routes.");
            free(new_route);
            return NULL;
        }

        int index = 0;
        cJSON *child;
        cJSON_ArrayForEach(child, children) {
            Route *child_route = parse_route_recursive(child, new_route->path);
            if (child_route) {
                new_route->children[index++] = child_route;
                new_route->child_count++;
            }
        }
    }

    return new_route;
}


void parse_routes(const char *json_str) {
    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        log_error("Failed to parse routes.json. JSON is malformed.");
        return;
    }

    if (!routes) {
        routes = malloc(INITIAL_ROUTES_SIZE * sizeof(Route *));
        if (!routes) {
            log_error("Memory allocation failed for routes array.");
            cJSON_Delete(json);
            return;
        }
    }

    int index = 0;
    cJSON *route;
    cJSON_ArrayForEach(route, json) {
        if (index >= INITIAL_ROUTES_SIZE) break;

        Route *new_route = parse_route_recursive(route, "");
        if (new_route) {
            routes[routes_count++] = new_route;
        }
    }

    cJSON_Delete(json);
}


char *resolve_route_recursive(Route *route, const char *request_path) {
    if (strcmp(route->path, request_path) == 0) {
        log_debug("Route %s has been resolved.", route->path);
        return strdup(route->view);
    }

    for (int i = 0; i < route->child_count; i++) {
        char *resolved_view = resolve_route_recursive(route->children[i], request_path);
        if (resolved_view) return resolved_view;
    }

    return NULL;
}


char *resolve_route(const char *request_path) {
    for (int i = 0; i < routes_count; i++) {
        char *view = resolve_route_recursive(routes[i], request_path);
        if (view) return view;
    }
    return NULL;
}
