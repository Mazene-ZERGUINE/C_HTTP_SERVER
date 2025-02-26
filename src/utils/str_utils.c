#include "../../includes/str_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void read_line(char *input, size_t size) {
    if (!fgets(input, size, stdin)) {
        return;  // Handle input failure (e.g., Ctrl+D)
    }

    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == size - 1 && input[size - 2] != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Consume remaining input
    }
}

char *extract_token(const char **str, const char *delimiters) {
    if (*str == NULL) return NULL;
    *str += strspn(*str, delimiters);

    if (**str == '\0') return NULL;
    const char *end = *str + strcspn(*str, delimiters);

    size_t length = end - *str;
    char *token = (char *)malloc(length + 1);
    if (!token) return NULL;

    strncpy(token, *str, length);
    token[length] = '\0';

    *str = (*end != '\0') ? end + 1 : end;
    return token;
}

char **str_split(const char *str, const char *delimiters, int *count) {
    if (!str || !delimiters) return NULL;

    int num_tokens = 0;
    const char *temp = str;
    while (*temp) {
        temp += strspn(temp, delimiters);
        if (*temp) {
            num_tokens++;
            temp += strcspn(temp, delimiters);
        }
    }

    if (num_tokens == 0) {
        *count = 0;
        return NULL;
    }

    char **result = (char **)malloc((num_tokens + 1) * sizeof(char *));
    if (!result) return NULL;

    temp = str;
    int index = 0;
    while (*temp) {
        temp += strspn(temp, delimiters);
        if (*temp) {
            const char *end = temp + strcspn(temp, delimiters);
            size_t length = end - temp;

            result[index] = (char *)malloc(length + 1);
            if (!result[index]) {
                for (int i = 0; i < index; i++) free(result[i]);
                free(result);
                return NULL;
            }

            strncpy(result[index], temp, length);
            result[index][length] = '\0';
            index++;
            temp = (*end) ? end + 1 : end;
        }
    }

    result[num_tokens] = NULL;
    *count = num_tokens;

    return result;
}

void free_split(char **tokens) {
    if (!tokens) return;
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

void array_len(char **str_array, size_t *length) {
    while (str_array[*length] != NULL) {
        (*length)++;
    }
}
