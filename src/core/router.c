#include "../../includes/router.h"

#include "server.h"

#include <sys/stat.h>  // For checking file modification time

void load_routes(const Server *server) {
    static time_t last_modified = 0;  // ✅ Track last modified timestamp
    struct stat file_stat;

    char *routes_file_path = strdup(server->app_config->app_resources_path);
    strcat(routes_file_path, "/app.routes.json");

    // ✅ Check last modification time
    if (stat(routes_file_path, &file_stat) == 0) {
        if (file_stat.st_mtime <= last_modified) {
            free(routes_file_path);
            return;  // ✅ No changes detected, skip reload
        }
        last_modified = file_stat.st_mtime;  // ✅ Update last modified time
    } else {
        log_error("Failed to access app.routes.json. Server shutdown...");
        free(routes_file_path);
        exit(EXIT_FAILURE);
    }

    log_info("Reloading routes from file...");

    FILE *file = fopen(routes_file_path, "r");
    if (!file) {
        log_error("app.routes.json not found. Server shutdown...");
        free(routes_file_path);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *json_str = malloc(file_size + 1);
    fread(json_str, 1, file_size, file);
    json_str[file_size] = '\0';
    fclose(file);
    free(routes_file_path);

    parse_routes(json_str);
    free(json_str);
}


char *remove_trailing_slash(const char *path) {
    size_t len = strlen(path);
    if (len > 1 && path[len - 1] == '/') {
        char *clean_path = strndup(path, len - 1);
        return clean_path;
    }
    return strdup(path);
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

        // ✅ Ensure "path" and "view" keys exist before using them
        cJSON *path_item = cJSON_GetObjectItem(route, "path");
        cJSON *view_item = cJSON_GetObjectItem(route, "view");

        if (!path_item || !cJSON_IsString(path_item)) {
            log_error("Malformed routes.json: Missing or invalid 'path'.");
            cJSON_Delete(json);
            exit(EXIT_FAILURE);
        }
        if (!view_item || !cJSON_IsString(view_item)) {
            log_error("Malformed routes.json: Missing or invalid 'view'.");
            cJSON_Delete(json);
            exit(EXIT_FAILURE);
        }

        Route *new_route = (Route *)malloc(sizeof(Route));
        if (!new_route) {
            log_error("Memory allocation failed for new route.");
            cJSON_Delete(json);
            return;
        }

        new_route->path = remove_trailing_slash(path_item->valuestring);
        new_route->view = strdup(view_item->valuestring);
        new_route->child_count = 0;
        new_route->children = NULL;

        cJSON *children = cJSON_GetObjectItem(route, "children");
        if (children && cJSON_IsArray(children)) {
            int child_index = 0;
            new_route->children = malloc(INITIAL_ROUTES_SIZE * sizeof(Route *));
            if (!new_route->children) {
                log_error("Memory allocation failed for child routes.");
                free(new_route);
                return;
            }

            for (int i = 0; i < INITIAL_ROUTES_SIZE; i++) {
                new_route->children[i] = NULL;
            }

            cJSON *child;
            cJSON_ArrayForEach(child, children) {
                if (child_index >= INITIAL_ROUTES_SIZE) break;

                // ✅ Ensure "path" and "view" exist in child
                cJSON *child_path_item = cJSON_GetObjectItem(child, "path");
                cJSON *child_view_item = cJSON_GetObjectItem(child, "view");

                if (!child_path_item || !cJSON_IsString(child_path_item)) {
                    log_error("Malformed routes.json: Missing or invalid 'path' in child route.");
                    cJSON_Delete(json);
                    exit(EXIT_FAILURE);
                }
                if (!child_view_item || !cJSON_IsString(child_view_item)) {
                    log_error("Malformed routes.json: Missing or invalid 'view' in child route.");
                    cJSON_Delete(json);
                    exit(EXIT_FAILURE);
                }

                Route *child_route = (Route *)malloc(sizeof(Route));
                if (!child_route) {
                    log_error("Memory allocation failed for child route.");
                    continue;
                }

                char full_child_path[256];
                snprintf(full_child_path, sizeof(full_child_path), "%s%s", new_route->path, child_path_item->valuestring);
                child_route->path = strdup(full_child_path);
                child_route->view = strdup(child_view_item->valuestring);
                child_route->child_count = 0;
                child_route->children = NULL;

                new_route->children[child_index] = child_route;
                new_route->child_count++;
                child_index++;
            }
        }

        if (routes_count >= INITIAL_ROUTES_SIZE) {
            log_error("Routes array is full. Increase INITIAL_ROUTES_SIZE.");
            free(new_route->path);
            free(new_route->view);
            free(new_route);
            cJSON_Delete(json);
            return;
        }

        routes[routes_count] = new_route;
        routes_count++;
        index++;

    }
    cJSON_Delete(json);
}



char *resolve_route(const char *request_path) {
    for (int i = 0; i < routes_count; i++) {
        if (strcmp(routes[i]->path, request_path) == 0) {
            log_debug("Route %s has been resolved.", routes[i]->path);
            return strdup(routes[i]->view);
        }

        for (int j = 0; j < routes[i]->child_count; j++) {
            if (strcmp(routes[i]->children[j]->path, request_path) == 0) {
                return strdup(routes[i]->children[j]->view);
            }
        }
    }
    return NULL;
}
