#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <stdlib.h>
#include <string.h>

#define CONTENT_LENGTH_BUFFER 20

/**
 * @file str_utils.h
 * @brief Utility functions for string processing.
 *
 * This file provides functions for handling and manipulating strings,
 * including token extraction.
 */

/**
 * @brief Extracts the next token from a string.
 *
 * This function extracts a token from the given string based on specified delimiters.
 * It updates the input pointer to point past the extracted token, allowing for
 * iterative parsing.
 *
 * @param str A pointer to the string pointer. It is updated to point past the extracted token.
 * @param delimiters A string containing delimiter characters that define token boundaries.
 * @return A dynamically allocated string containing the extracted token. The caller must free it.
 */

void read_line(char *input, size_t size);

char *extract_token(const char **str, const char *delimiters);
char* size_t_to_string(const size_t file_size);

char **str_split(const char *str, const char *delimiters, int *count);
void free_split(char** str_array);

void array_len(char **str_array, size_t *length);

#endif // STR_UTILS_H
