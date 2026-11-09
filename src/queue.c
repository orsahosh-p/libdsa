#include "dsa/queue.h"
#include "dsa/linkedlist.h"
#include <stdlib.h>

struct Queue {
    LinkedList *storage;
};

Status queue_create(size_t element_size, Queue **out){
    if(out == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    *out = NULL;

    if(element_size == 0){
        return STATUS_INVALID_ARGUMENT;
    }

    Queue* queue = malloc(sizeof(*queue));

    if(queue == NULL){
        return STATUS_OUT_OF_MEMORY;
    }

    LinkedList* linkedlist = NULL;

    Status status = linkedlist_create(element_size, &linkedlist);

    if(status != STATUS_OK){
        free(queue);
        return status;
    }

    queue->storage = linkedlist;

    *out = queue;

    return STATUS_OK;
}

void queue_destroy(Queue *queue){

    if(queue == NULL){
        return;
    }

    linkedlist_destroy(queue->storage);

    free(queue);
}

Status queue_add(Queue *queue, const void *element){
    if(queue == NULL || element == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    return linkedlist_add(queue->storage, element);
}

Status queue_remove(Queue *queue){
    if(queue == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(linkedlist_size(queue->storage) == 0){
        return STATUS_EMPTY;
    }

    size_t index = 0;

    return linkedlist_remove(queue->storage, index);
}

Status queue_get(const Queue *queue, void *out){
    if(queue == NULL || out == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(linkedlist_size(queue->storage) == 0){
        return STATUS_EMPTY;
    }

    size_t index = 0;

    return linkedlist_get(queue->storage, index, out);
}

size_t queue_size(const Queue *queue){
    if(queue == NULL){
        return 0;
    }

    return linkedlist_size(queue->storage);
}

bool queue_is_empty(const Queue *queue){
    if(queue == NULL){
        return true;
    }

    return linkedlist_is_empty(queue->storage);
}

Status queue_clear(Queue *queue){
    if(queue == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    return linkedlist_clear(queue->storage);
}
