#include "../../includes/memory_utils.h"

#include <stdio.h>
#include <stdlib.h>

void realloc_buffer(char** buffer, unsigned long new_size) {
    char* tmp = realloc(buffer, new_size);
    if (!tmp) {
        fprintf(stderr, "Error allocating memory for new buffer\n");
        return;
    }
    *buffer = tmp;
}

void copy_file(const char *src, const char *dest) {
    FILE *source = fopen(src, "rb");
    if (!source) {
        perror("Error opening source file");
        return;
    }

    FILE *destination = fopen(dest, "wb");
    if (!destination) {
        perror("Error opening destination file");
        fclose(source);
        return;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytes, destination);
    }

    fclose(source);
    fclose(destination);
    printf("\033[1;32m✔ Copied %s to %s\033[0m\n", src, dest);
}