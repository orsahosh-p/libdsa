#include "dsa/vector.h"
#include <stdlib.h>
#include <string.h>

struct Vector {
    void *data;
    size_t element_size;
    size_t size;
    size_t capacity;
};

Status vector_create(size_t element_size, Vector **out)
{
    if(out == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    *out = NULL;

    if(element_size == 0){
        return STATUS_INVALID_ARGUMENT;
    }

    Vector *vector = malloc(sizeof(*vector));

    if(vector == NULL){
        return STATUS_OUT_OF_MEMORY;
    }

    vector->data = NULL;
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = 0;

    *out = vector;

    return STATUS_OK;
}

void vector_destroy(Vector *vector)
{
    if(vector == NULL){
        return;
    }

    free(vector->data);
    free(vector);
}

Status vector_clear(Vector *vector)
{
    if(vector == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    vector->size = 0;

    return STATUS_OK;
}

size_t vector_size(const Vector *vector){
    if(vector == NULL){
        return 0;
    }

    return vector->size;
}

bool vector_is_empty(const Vector *vector){
    if(vector == NULL || vector->size == 0){
        return true;
    }

    return false;
}

size_t vector_capacity(const Vector *vector){
    if(vector == NULL){
        return 0;
    }

    return vector->capacity;
}

Status vector_reserve(Vector *vector, size_t capacity){
    if(vector == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(capacity <= vector->capacity){
        return STATUS_OK;
    }

    if(capacity > (SIZE_MAX / vector->element_size)){
        return STATUS_OUT_OF_MEMORY;
    }

    void *new_data = realloc(vector->data, capacity * vector->element_size);

    if(new_data == NULL){
        return STATUS_OUT_OF_MEMORY;
    }

    vector->data = new_data;
    vector->capacity = capacity;

    return STATUS_OK;
}

Status vector_add(Vector *vector, const void *element){
    if(vector == NULL || element == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(vector->size == vector->capacity){
        size_t new_capacity;
        
        if(vector->capacity == 0){
            new_capacity = 4;
        }else{
            if(vector->capacity > SIZE_MAX / 2){
                return STATUS_OUT_OF_MEMORY;
            }

            new_capacity = vector->capacity * 2;
        }

        Status status = vector_reserve(vector, new_capacity);

        if(status != STATUS_OK){
            return status;
        }
    }

    size_t offset = vector->size * vector->element_size;

    unsigned char *destination = (unsigned char *)vector->data + offset;

    memcpy(destination, element, vector->element_size);

    vector->size += 1;

    return STATUS_OK;
}

Status vector_remove(Vector *vector, size_t index){
    if(vector == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(index >= vector->size){
        return STATUS_OUT_OF_BOUNDS;
    }

    size_t offset = index * vector->element_size;

    unsigned char *destination = (unsigned char *)vector->data + offset;

    size_t bytes = (vector->size - index - 1) * vector->element_size;

    if(bytes != 0){
        memmove(destination, destination + vector->element_size, bytes);
    }

    vector->size -= 1;

    return STATUS_OK;
}

Status vector_get(const Vector *vector, size_t index, void *out){
    if(vector == NULL || out == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(index >= vector->size){
        return STATUS_OUT_OF_BOUNDS;
    }

    size_t offset = index * vector->element_size;

    const unsigned char *source = (const unsigned char *)vector->data + offset;

    memcpy(out, source, vector->element_size);

    return STATUS_OK;
}

Status vector_set(Vector *vector, size_t index, const void *element)
{
    if (vector == NULL || element == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (index >= vector->size) {
        return STATUS_OUT_OF_BOUNDS;
    }

    size_t offset = index * vector->element_size;

    unsigned char *destination =
        (unsigned char *)vector->data + offset;

    memcpy(destination, element, vector->element_size);

    return STATUS_OK;
}

