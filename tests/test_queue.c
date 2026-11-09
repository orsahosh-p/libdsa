#include "dsa/queue.h"

#include <assert.h>
#include <stdio.h>

static void test_create_and_invalid_arguments(void)
{
    Queue *queue = NULL;
    int value = 10;
    int out = -1;

    assert(queue_create(sizeof(int), NULL) == STATUS_INVALID_ARGUMENT);
    assert(queue_create(0, &queue) == STATUS_INVALID_ARGUMENT);
    assert(queue == NULL);
    assert(queue_add(NULL, &value) == STATUS_INVALID_ARGUMENT);
    assert(queue_get(NULL, &out) == STATUS_INVALID_ARGUMENT);
    assert(queue_remove(NULL) == STATUS_INVALID_ARGUMENT);
    assert(queue_clear(NULL) == STATUS_INVALID_ARGUMENT);
    assert(queue_size(NULL) == 0);
    assert(queue_is_empty(NULL));
    queue_destroy(NULL);

    assert(queue_create(sizeof(int), &queue) == STATUS_OK);
    assert(queue != NULL);
    assert(queue_size(queue) == 0);
    assert(queue_is_empty(queue));
    Queue *failed_output = queue;
    assert(queue_create(0, &failed_output) == STATUS_INVALID_ARGUMENT);
    assert(failed_output == NULL);
    assert(queue_add(queue, NULL) == STATUS_INVALID_ARGUMENT);
    assert(queue_get(queue, NULL) == STATUS_INVALID_ARGUMENT);
    assert(queue_get(queue, &out) == STATUS_EMPTY);
    assert(out == -1);
    assert(queue_remove(queue) == STATUS_EMPTY);
    assert(queue_size(queue) == 0);
    assert(queue_add(queue, &value) == STATUS_OK);
    assert(queue_add(queue, NULL) == STATUS_INVALID_ARGUMENT);
    assert(queue_get(queue, NULL) == STATUS_INVALID_ARGUMENT);
    assert(queue_size(queue) == 1);
    assert(queue_get(queue, &out) == STATUS_OK);
    assert(out == value);
    queue_destroy(queue);
}

static void test_fifo(void)
{
    Queue *queue = NULL;
    assert(queue_create(sizeof(int), &queue) == STATUS_OK);
    for (int i = 0; i < 100; ++i) {
        assert(queue_add(queue, &i) == STATUS_OK);
        assert(queue_size(queue) == (size_t)i + 1);
    }
    assert(!queue_is_empty(queue));

    for (int i = 0; i < 100; ++i) {
        int out = -1;
        assert(queue_get(queue, &out) == STATUS_OK);
        assert(out == i);
        assert(queue_size(queue) == (size_t)(100 - i));
        out = -1;
        assert(queue_get(queue, &out) == STATUS_OK);
        assert(out == i);
        assert(queue_remove(queue) == STATUS_OK);
        assert(queue_size(queue) == (size_t)(99 - i));
    }
    int out = -1;
    assert(queue_is_empty(queue));
    assert(queue_get(queue, &out) == STATUS_EMPTY);
    assert(out == -1);
    assert(queue_remove(queue) == STATUS_EMPTY);
    queue_destroy(queue);
}

static void test_interleaved_operations_and_reuse(void)
{
    Queue *queue = NULL;
    int values[] = {10, 20, 30, 40};
    int out = 0;
    assert(queue_create(sizeof(int), &queue) == STATUS_OK);
    assert(queue_add(queue, &values[0]) == STATUS_OK);
    assert(queue_add(queue, &values[1]) == STATUS_OK);
    assert(queue_remove(queue) == STATUS_OK);
    assert(queue_add(queue, &values[2]) == STATUS_OK);
    assert(queue_size(queue) == 2);
    for (int i = 1; i <= 2; ++i) {
        assert(queue_get(queue, &out) == STATUS_OK);
        assert(out == values[i]);
        assert(queue_remove(queue) == STATUS_OK);
    }
    assert(queue_is_empty(queue));

    /* Repeatedly transition between empty and a single element. */
    for (int i = 0; i < 4; ++i) {
        assert(queue_add(queue, &values[i]) == STATUS_OK);
        assert(queue_size(queue) == 1);
        assert(queue_get(queue, &out) == STATUS_OK);
        assert(out == values[i]);
        assert(queue_remove(queue) == STATUS_OK);
        assert(queue_is_empty(queue));
    }
    queue_destroy(queue);
}

static void test_clear_and_reuse(void)
{
    Queue *queue = NULL;
    int value = 7;
    int out = -1;
    assert(queue_create(sizeof(int), &queue) == STATUS_OK);
    assert(queue_clear(queue) == STATUS_OK);
    assert(queue_add(queue, &value) == STATUS_OK);
    assert(queue_add(queue, &value) == STATUS_OK);
    assert(queue_clear(queue) == STATUS_OK);
    assert(queue_size(queue) == 0);
    assert(queue_is_empty(queue));
    assert(queue_get(queue, &out) == STATUS_EMPTY);
    assert(queue_remove(queue) == STATUS_EMPTY);
    assert(queue_clear(queue) == STATUS_OK);
    value = 99;
    assert(queue_add(queue, &value) == STATUS_OK);
    value = 100;
    assert(queue_add(queue, &value) == STATUS_OK);
    assert(queue_size(queue) == 2);
    assert(queue_get(queue, &out) == STATUS_OK);
    assert(out == 99);
    assert(queue_remove(queue) == STATUS_OK);
    assert(queue_get(queue, &out) == STATUS_OK);
    assert(out == 100);
    queue_destroy(queue);
}

static void test_struct_copy(void)
{
    typedef struct {
        int id;
        double value;
    } Item;
    Queue *queue = NULL;
    Item input = {5, 3.5};
    Item output = {0};
    assert(queue_create(sizeof(Item), &queue) == STATUS_OK);
    assert(queue_add(queue, &input) == STATUS_OK);
    input.id = 99;
    input.value = 0;
    assert(queue_get(queue, &output) == STATUS_OK);
    assert(output.id == 5 && output.value == 3.5);
    output.id = -1;
    assert(queue_get(queue, &output) == STATUS_OK);
    assert(output.id == 5);
    queue_destroy(queue);
}

int main(void)
{
    test_create_and_invalid_arguments();
    test_fifo();
    test_interleaved_operations_and_reuse();
    test_clear_and_reuse();
    test_struct_copy();
    puts("All Queue tests passed!");
    return 0;
}
