#ifndef ROUTER_H
#define ROUTER_H

#define INITIAL_ROUTES_SIZE 100
#include <cjson/cJSON.h>
#include "logger.h"
#include "server.h"


typedef struct Route {
    char* path;
    char *view;
    struct Route **children;
    int child_count;
} Route;

static Route **routes;
static int routes_count = 0;


void parse_routes(const char *json_str);
void load_routes(const Server* server);
char *resolve_route(const char *request_path);

#endif //ROUTER_H
