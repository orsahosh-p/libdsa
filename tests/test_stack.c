#include "dsa/stack.h"

#include <assert.h>
#include <stdio.h>

static void test_create_and_invalid_arguments(void)
{
    Stack *stack = NULL;
    int value = 10;
    int out = 0;

    assert(stack_create(sizeof(int), NULL) == STATUS_INVALID_ARGUMENT);
    assert(stack_create(0, &stack) == STATUS_INVALID_ARGUMENT);
    assert(stack == NULL);
    assert(stack_add(NULL, &value) == STATUS_INVALID_ARGUMENT);
    assert(stack_get(NULL, &out) == STATUS_INVALID_ARGUMENT);
    assert(stack_remove(NULL) == STATUS_INVALID_ARGUMENT);
    assert(stack_clear(NULL) == STATUS_INVALID_ARGUMENT);
    assert(stack_size(NULL) == 0);
    assert(stack_is_empty(NULL));
    stack_destroy(NULL);

    assert(stack_create(sizeof(int), &stack) == STATUS_OK);
    assert(stack != NULL);
    assert(stack_size(stack) == 0);
    assert(stack_is_empty(stack));
    assert(stack_add(stack, NULL) == STATUS_INVALID_ARGUMENT);
    assert(stack_get(stack, NULL) == STATUS_INVALID_ARGUMENT);
    assert(stack_get(stack, &out) == STATUS_EMPTY);
    assert(stack_remove(stack) == STATUS_EMPTY);
    assert(stack_size(stack) == 0);
    stack_destroy(stack);
}

static void test_lifo_and_growth(void)
{
    Stack *stack = NULL;
    assert(stack_create(sizeof(int), &stack) == STATUS_OK);

    for (int i = 0; i < 100; ++i) {
        assert(stack_add(stack, &i) == STATUS_OK);
        assert(stack_size(stack) == (size_t)i + 1);
    }
    assert(!stack_is_empty(stack));

    for (int i = 99; i >= 0; --i) {
        int out = -1;
        assert(stack_get(stack, &out) == STATUS_OK);
        assert(out == i);
        assert(stack_size(stack) == (size_t)i + 1);
        out = -1;
        assert(stack_get(stack, &out) == STATUS_OK);
        assert(out == i);
        assert(stack_remove(stack) == STATUS_OK);
        assert(stack_size(stack) == (size_t)i);
    }

    int out = 0;
    assert(stack_is_empty(stack));
    assert(stack_get(stack, &out) == STATUS_EMPTY);
    assert(stack_remove(stack) == STATUS_EMPTY);
    int value = 42;
    assert(stack_add(stack, &value) == STATUS_OK);
    assert(stack_get(stack, &out) == STATUS_OK);
    assert(out == value);
    stack_destroy(stack);
}

static void test_clear_and_reuse(void)
{
    Stack *stack = NULL;
    int value = 7;
    int out = 0;
    assert(stack_create(sizeof(int), &stack) == STATUS_OK);
    assert(stack_clear(stack) == STATUS_OK);
    assert(stack_add(stack, &value) == STATUS_OK);
    assert(stack_add(stack, &value) == STATUS_OK);
    assert(stack_clear(stack) == STATUS_OK);
    assert(stack_is_empty(stack));
    assert(stack_size(stack) == 0);
    assert(stack_get(stack, &out) == STATUS_EMPTY);
    assert(stack_remove(stack) == STATUS_EMPTY);
    value = 99;
    assert(stack_add(stack, &value) == STATUS_OK);
    assert(stack_get(stack, &out) == STATUS_OK);
    assert(out == 99);
    assert(stack_size(stack) == 1);
    stack_destroy(stack);
}

static void test_struct_copy(void)
{
    typedef struct {
        int id;
        double value;
    } Item;

    Stack *stack = NULL;
    Item input = {5, 3.5};
    Item output = {0};
    assert(stack_create(sizeof(Item), &stack) == STATUS_OK);
    assert(stack_add(stack, &input) == STATUS_OK);
    input.id = 99;
    input.value = 0;
    assert(stack_get(stack, &output) == STATUS_OK);
    assert(output.id == 5);
    assert(output.value == 3.5);
    output.id = -1;
    assert(stack_get(stack, &output) == STATUS_OK);
    assert(output.id == 5);
    stack_destroy(stack);
}

int main(void)
{
    test_create_and_invalid_arguments();
    test_lifo_and_growth();
    test_clear_and_reuse();
    test_struct_copy();
    puts("All Stack tests passed!");
    return 0;
}
