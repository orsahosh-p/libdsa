#ifndef LIBDSA_COMMON_H
#define LIBDSA_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT,
    STATUS_OUT_OF_MEMORY,
    STATUS_OUT_OF_BOUNDS,
    STATUS_EMPTY,
    STATUS_FULL,
    STATUS_NOT_FOUND,
    STATUS_ALREADY_EXISTS
} Status;


typedef int (*Comparator)(
    const void *a,
    const void *b
);

typedef size_t (*HashFunction)(
    const void *key
);

typedef void (*Destructor)(
    void *element
);

#endif