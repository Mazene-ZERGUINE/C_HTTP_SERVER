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

