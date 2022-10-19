#ifndef STD_ERROR_H
#define STD_ERROR_H

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include "std/unique.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct std_error_t std_error_t;

typedef void (*std_error_log_format_callback)(const char **messages, size_t messages_count);
typedef char *(*std_error_get_format_callback)(const char **messages, size_t messages_count);

std_error_t *std_error_new(const char *message);
std_error_t *std_error_append(std_error_t *error, const char *message);

void std_error_log(std_error_t *error);
void std_error_log_to(std_error_t *error, FILE *stream);
void std_error_log_formatted(std_error_t *error, std_error_log_format_callback formatter);

char *std_error_dump(std_error_t *error);
char *std_error_dump_formatted(std_error_t *error, std_error_get_format_callback formatter);

const char *std_error_get(std_error_t *error, size_t index);
const char *std_error_first(std_error_t *error);
const char *std_error_last(std_error_t *error);

size_t std_error_count(std_error_t *error);

void std_error_free(std_error_t *error);
void std_error_free_safe(std_error_t **error);

declare_std_unique_ptr_for_type(std_error_t, std_error_free_safe)

#ifdef __cplusplus
}
#endif

#endif /* STD_ERROR_H */
