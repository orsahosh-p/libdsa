#include "dsa/linkedlist.h"

#include <stdlib.h>
#include <string.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

struct LinkedList {
    Node *head;
    Node *tail;
    size_t element_size;
    size_t size;
};

Status linkedlist_create(size_t element_size, LinkedList **out)
{
    if(out == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    *out = NULL;

    if(element_size == 0){
        return STATUS_INVALID_ARGUMENT;
    }

    LinkedList *list = malloc(sizeof(*list));

    if(list == NULL){
        return STATUS_OUT_OF_MEMORY;
    }

    list->element_size = element_size;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    *out = list;

    return STATUS_OK;
}

void linkedlist_destroy(LinkedList *list)
{
    if(list == NULL){
        return;
    }

    linkedlist_clear(list);
    free(list);
}

Status linkedlist_clear(LinkedList *list)
{
    if(list == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    while(list->head != NULL){
        Node *temp = list->head;
        list->head = list->head->next;
        free(temp->data);
        free(temp);
    }

    list->tail = NULL;
    list->size = 0;

    return STATUS_OK;
}

size_t linkedlist_size(const LinkedList *list)
{
    if(list == NULL){
        return 0;
    }

    return list->size;
}

bool linkedlist_is_empty(const LinkedList *list)
{
    return list == NULL || list->head == NULL;
}

Status linkedlist_add(LinkedList *list, const void *element){
    if(list == NULL || element == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(list->size == SIZE_MAX){
        return STATUS_OUT_OF_MEMORY;
    }

    Node* elem = malloc(sizeof(*elem));

    if(elem == NULL){
        return STATUS_OUT_OF_MEMORY;
    }
    
    elem->data = malloc(list->element_size);

    if(elem->data == NULL){
        free(elem);
        return STATUS_OUT_OF_MEMORY;
    }

    memcpy(elem->data, element, list->element_size);
    elem->next = NULL;

    if(list->head == NULL){
        list->head = elem;
    }else{
        list->tail->next = elem;
    }

    list->tail = elem;
    list->size += 1;

    return STATUS_OK;
}

Status linkedlist_remove(LinkedList *list, size_t index){
    if(list == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(index >= list->size){
        return STATUS_OUT_OF_BOUNDS;
    }

    Node* removed;

    if(index == 0){
        removed = list->head;
        list->head = removed->next;
        if(list->head == NULL){
            list->tail = NULL;
        }
    }else{
        Node* previous = list->head;

        for(size_t i = 0; i < index - 1; i++){
            previous = previous->next;
        }

        removed = previous->next;
        previous->next = removed->next;
        if(removed == list->tail){
            list->tail = previous;
        }
    }

    free(removed->data);
    free(removed);
    list->size -= 1;

    return STATUS_OK;
}

Status linkedlist_get(const LinkedList *list, size_t index, void *out){
    if(list == NULL || out == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(index >= list->size){
        return STATUS_OUT_OF_BOUNDS;
    }

    const Node *node = list->head;

    for(size_t i = 0; i < index; i++){
        node = node->next;
    }

    memcpy(out, node->data, list->element_size);

    return STATUS_OK;
}

Status linkedlist_set(LinkedList *list, size_t index, const void *element){
    if(list == NULL || element == NULL){
        return STATUS_INVALID_ARGUMENT;
    }

    if(index >= list->size){
        return STATUS_OUT_OF_BOUNDS;
    }

    Node *node = list->head;

    for(size_t i = 0; i < index; i++){
        node = node->next;
    }

    memcpy(node->data, element, list->element_size);

    return STATUS_OK;
}
