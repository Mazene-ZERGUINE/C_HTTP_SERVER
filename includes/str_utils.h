#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <stdlib.h>
#include <string.h>

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
char *extract_token(const char **str, const char *delimiters);

#endif // STR_UTILS_H
