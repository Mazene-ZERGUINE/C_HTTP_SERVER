#include "../../includes/router.h"

#include "server.h"

void load_routes(const Server *server) {
    char *routes_file_path = strdup(server->app_config->app_resources_path);
    if (!routes_file_path) {
        log_error("Memory allocation failed for routes file path");
        return;
    }
    const char *file_name = "/app.routes.json";

    char *new_path = realloc(routes_file_path, strlen(routes_file_path) + strlen(file_name) + 1);
    if (!new_path) {
        log_error("Memory reallocation failed for routes file path");
        free(routes_file_path);
        return;
    }
    routes_file_path = new_path;

    strcat(routes_file_path, file_name);

    FILE *file = fopen(routes_file_path, "r");
    if (!file) {
        log_error("app.routes.json not found. Routing file must be at the root level of your app.");
        log_error("Server shutdown ....");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *json_str = (char *)malloc(file_size + 1);
    if (!json_str) {
        log_error("Memory allocation failed for JSON buffer.");
        fclose(file);
        return;
    }

    fread(json_str, 1, file_size, file);
    json_str[file_size] = '\0';
    fclose(file);
    parse_routes(json_str);
    free(json_str);
}

char *remove_trailing_slash(const char *path) {
    size_t len = strlen(path);
    if (len > 1 && path[len - 1] == '/') {  // Avoid removing the only slash in "/"
        char *clean_path = strndup(path, len - 1);
        return clean_path;
    }
    return strdup(path);
}


void parse_routes(const char *json_str) {
    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        log_error("Failed to parse routes.json.");
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

        Route *new_route = (Route *)malloc(sizeof(Route));
        if (!new_route) {
            log_error("Memory allocation failed for new route.");
            cJSON_Delete(json);
            return;
        }

        new_route->path = remove_trailing_slash(cJSON_GetObjectItem(route, "path")->valuestring);
        new_route->component = strdup(cJSON_GetObjectItem(route, "view")->valuestring);
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

                Route *child_route = (Route *)malloc(sizeof(Route));
                if (!child_route) {
                    log_error("Memory allocation failed for child route.");
                    continue;
                }

                char full_child_path[256];
                snprintf(full_child_path, sizeof(full_child_path), "%s%s", new_route->path, cJSON_GetObjectItem(child, "path")->valuestring);
                child_route->path = strdup(full_child_path);
                child_route->component = strdup(cJSON_GetObjectItem(child, "view")->valuestring);
                child_route->child_count = 0;
                child_route->children = NULL;

                new_route->children[child_index] = child_route;
                new_route->child_count++;
                child_index++;
            }
        }

        routes[routes_count] = new_route;
        routes_count++;
        index++;
    }

    cJSON_Delete(json);
    log_info("Routes successfully loaded from JSON.");
}


char *resolve_route(const char *request_path) {
    for (int i = 0; i < routes_count; i++) {
        if (strcmp(routes[i]->path, request_path) == 0) {
            return strdup(routes[i]->component);
        }

        for (int j = 0; j < routes[i]->child_count; j++) {
            log_debug("Checking child route: %s", routes[i]->children[j]->path);

            if (strcmp(routes[i]->children[j]->path, request_path) == 0) {
                return strdup(routes[i]->children[j]->component);
            }
        }
    }
    return NULL;
}
