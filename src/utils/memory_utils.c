#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

/**
 * @file memory_utils.h
 * @brief Utility functions for dynamic memory management.
 *
 * This file contains function prototypes for handling dynamic memory allocation
 * operations such as resizing buffers.
 */

/**
 * @brief Reallocates a buffer to a new size.
 *
 * This function dynamically resizes a given buffer to the specified new size.
 * It prevents memory leaks and ensures proper reallocation handling.
 *
 * @param buffer A pointer to the buffer that needs resizing.
 * @param new_size The new size in bytes for the buffer.
 */
void realloc_buffer(char** buffer, unsigned long new_size);

#endif // MEMORY_UTILS_H
