//
// Created by Mazene ZERGUINZ on 22/02/2025.
//

#include "../../includes/str_utils.h"


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
