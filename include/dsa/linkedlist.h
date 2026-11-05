#ifndef LIBDSA_LINKEDLIST_H
#define LIBDSA_LINKEDLIST_H

#include "common.h"

typedef struct LinkedList LinkedList;

Status linkedlist_create(size_t element_size, LinkedList **out);
void linkedlist_destroy(LinkedList *list);
Status linkedlist_clear(LinkedList *list);

Status linkedlist_add(LinkedList *list, const void *element);
Status linkedlist_remove(LinkedList *list, size_t index);
Status linkedlist_get(const LinkedList *list, size_t index, void *out);
Status linkedlist_set(LinkedList *list, size_t index, const void *element);

size_t linkedlist_size(const LinkedList *list);
bool linkedlist_is_empty(const LinkedList *list);

#endif
