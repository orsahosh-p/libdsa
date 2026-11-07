#ifndef LIBDSA_STACK_H
#define LIBDSA_STACK_H

#include "common.h"

typedef struct Stack Stack;

Status stack_create(size_t element_size, Stack **out);
void stack_destroy(Stack *stack);

Status stack_add(Stack *stack, const void *element);

Status stack_get(const Stack *stack, void *out);

Status stack_remove(Stack *stack);

size_t stack_size(const Stack *stack);

bool stack_is_empty(const Stack *stack);

Status stack_clear(Stack *stack);

#endif