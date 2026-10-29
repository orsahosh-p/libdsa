#ifndef LIBDSA_VECTOR_H
#define LIBDSA_VECTOR_H

#include "common.h"

typedef struct Vector Vector;

Status vector_create(size_t element_size, Vector **out);
void vector_destroy(Vector *vector);
Status vector_clear(Vector *vector);

Status vector_add(Vector *vector, const void *element);
Status vector_remove(Vector *vector, size_t index);
Status vector_get(const Vector *vector, size_t index, void *out);
Status vector_set(Vector *vector, size_t index, const void *element);

size_t vector_size(const Vector *vector);
bool vector_is_empty(const Vector *vector);
size_t vector_capacity(const Vector *vector);

Status vector_reserve(Vector *vector, size_t capacity);

#endif 