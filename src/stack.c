#include "dsa/stack.h"
#include "dsa/vector.h"
#include <stdlib.h>

struct Stack {
    Vector *storage;
};

Status stack_create(size_t element_size, Stack **out){
    if(out == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    *out = NULL;

    if(element_size == 0){
        return STATUS_INVALID_ARGUMENT;
    }

    Stack *stack = malloc(sizeof(*stack));

    if(stack == NULL){
        return STATUS_OUT_OF_MEMORY;
    }

    Vector *vector = NULL;

    Status status = vector_create(element_size, &vector);

    if(status != STATUS_OK){
        free(stack);
        return status;
    }

    stack->storage = vector;

    *out = stack;

    return STATUS_OK;
}

void stack_destroy(Stack *stack){
    if(stack == NULL){
        return;
    }

    vector_destroy(stack->storage);

    free(stack);
}

Status stack_add(Stack *stack, const void *element){
    if(stack == NULL || element == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    return vector_add(stack->storage, element);
}

Status stack_get(const Stack *stack, void *out){
    if(stack == NULL || out == NULL){
        return STATUS_INVALID_ARGUMENT;
    }   

    if(vector_size(stack->storage) == 0){
        return STATUS_EMPTY;
    }

    size_t index = vector_size(stack->storage) - 1;

    return vector_get(stack->storage, index, out);
}

Status stack_remove(Stack *stack){
    if(stack == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(vector_size(stack->storage) == 0){
        return STATUS_EMPTY;
    }
    
    size_t index = vector_size(stack->storage) - 1;

    return vector_remove(stack->storage, index);
}

size_t stack_size(const Stack *stack){
    if(stack == NULL){
        return 0;
    }

    return vector_size(stack->storage);
}

bool stack_is_empty(const Stack *stack){
    if(stack == NULL){
        return true;
    }

    return vector_size(stack->storage) == 0;
}

Status stack_clear(Stack *stack){
    if(stack == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    return vector_clear(stack->storage);
}