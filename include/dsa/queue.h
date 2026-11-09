#ifndef LIBDSA_QUEUE_H
#define LIBDSA_QUEUE_H

#include "common.h"

typedef struct Queue Queue;

Status queue_create(size_t element_size, Queue **out);
void queue_destroy(Queue *queue);

Status queue_add(Queue *queue, const void *element);
Status queue_remove(Queue *queue);
Status queue_get(const Queue *queue, void *out);

size_t queue_size(const Queue *queue);
bool queue_is_empty(const Queue *queue);
Status queue_clear(Queue *queue);

#endif