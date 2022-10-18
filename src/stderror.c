#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "stderror.h"

struct std_error_t {
    char **messages;
    size_t capacity;
    size_t size;
};

static char *default_std_error_get_callback(const char **messages, size_t messages_count) {
    const char *preamble = "[error]" "\n";
    size_t preamble_length = strlen(preamble);

    const char *tabulation = "   - ";
    size_t tabulation_length = strlen(tabulation);

    size_t result_length = preamble_length; /* preamble */
    result_length += messages_count * tabulation_length; /* tabulations */
    result_length += messages_count; /* end of lines */
    result_length += 1; /* end of string */

    for (size_t idx = 0; idx < messages_count; idx++)
        result_length += strlen(messages[idx]);

    char *result = calloc(result_length, sizeof(char));
    if (!result)
        return NULL;

    size_t cursor = 0;

    strncpy(result, preamble, preamble_length);
    cursor += preamble_length;

    for (size_t idx = 0; idx < messages_count; idx++) {
        size_t message_length = strlen(messages[idx]);

        strncpy(result + cursor, tabulation, tabulation_length);
        cursor += tabulation_length;

        strncpy(result + cursor, messages[idx], message_length);
        cursor += message_length;

        result[cursor] = '\n';
        cursor += 1;
    }

    return result;
}

std_error_t *std_error_new(const char *message) {
    std_error_t *error = calloc(1, sizeof(std_error_t));
    if (!error)
        return NULL;

    return std_error_append(error, message);
}

std_error_t *std_error_append(std_error_t *error, const char *message)
{
    assert(error && "attempt to append an error, but the error is null");

    if (error->size == error->capacity) {
        size_t next_capacity = error->capacity ? error->capacity * 2 : 8;
        char **messages_backup = error->messages;

        error->messages = realloc(error->messages, next_capacity * sizeof(char *));
        if (!error->messages) {
            fprintf(stderr, "[stderror] couldn't append an error (realloc failed)" "\n");

            error->messages = messages_backup;
            return error;
        }

        error->capacity = next_capacity;
    }

    size_t message_length = strlen(message);

    error->messages[error->size] = calloc(message_length + 1, sizeof(char));
    if (!error->messages[error->size]) {
        fprintf(stderr, "[stderror] couldn't append an error (strdup failed)" "\n");
        return error;
    }

    strncpy(error->messages[error->size], message, message_length);
    error->size += 1;

    return error;
}

void std_error_log(std_error_t *error) {
    std_error_log_to(error, stderr);
}

void std_error_log_to(std_error_t *error, FILE *stream)
{
    assert(error && "attempt to log an error, but the error is null");

    fprintf(stream, "[error]" "\n");

    for (size_t idx = 0; idx < error->size; idx++)
        fprintf(stream, "   - %s" "\n", error->messages[idx]);
}

void std_error_log_formatted(std_error_t *error, std_error_log_format_callback formatter) {
    assert(error && "attempt to log an error, but the error is null");

    formatter((const char **) error->messages, error->size);
}

char *std_error_dump(std_error_t *error) {
    assert(error && "attempt to get an error, but the error is null");

    return std_error_dump_formatted(error, default_std_error_get_callback);
}

char *std_error_dump_formatted(std_error_t *error, std_error_get_format_callback formatter) {
    assert(error && "attempt to get an error, but the error is null");

    return formatter((const char **) error->messages, error->size);
}

const char *std_error_get(std_error_t *error, size_t index) {
    assert(error && "attempt to get an error message by index, but the error is null");
    assert(error->size > index && "attempt to get an error message by index, but index is invalid");

    return (const char *) error->messages[index];
}

const char *std_error_first(std_error_t *error) {
    assert(error && "attempt to get an error message by index, but the error is null");
    assert(error->size > 0 && "attempt to get an error message by index, but the error is empty");

    return (const char *) error->messages[0];
}

const char *std_error_last(std_error_t *error) {
    assert(error && "attempt to get an error message by index, but the error is null");
    assert(error->size > 0 && "attempt to get an error message by index, but the error is empty");

    size_t count = std_error_count(error);
    return (const char *) error->messages[count - 1];
}

size_t std_error_count(std_error_t *error)
{
    assert(error && "attempt to get an error message by index, but the error is null");

    return error->size;
}

void std_error_free(std_error_t *error) {
    assert(error && "attempt to free an error, but the error is null");

    for (size_t idx = 0; idx < error->size; idx++)
        free(error->messages[idx]);

    if (error->messages)
        free(error->messages);

    free(error);
}

void std_error_free_safe(std_error_t **error) {
    assert(error && "attempt to free an error, but the error pointer is null");

    std_error_free(*error);
    *error = NULL;
}
