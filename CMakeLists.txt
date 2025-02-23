cmake_minimum_required(VERSION 3.16)

project(c_http_server C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(includes)

file(GLOB SOURCES "src/*.c")

add_executable(http_server ${SOURCES}
        src/core/server.c
        includes/server.h
        src/core/config.h
        src/handlers/client_request_handler.c
        includes/client_request_handler.h
        src/http/http_request_parser.c
        includes/http_request_parser.h
        src/utils/memory_utils.c
        includes/memory_utils.h
        src/utils/str_utils.c
        includes/str_utils.h
        src/http/http-response.c
        includes/http-response.h
        src/handlers/http_response_handler.c
        includes/http_response_handler.h
        src/core/logger.c
        includes/logger.h
)

add_custom_command(
        TARGET http_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/http_server  # Source location
        ${CMAKE_SOURCE_DIR}/bin/http_server           # Destination location
)


target_compile_options(http_server PRIVATE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O2>
)

target_compile_options(http_server PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(http_server pthread)
