#include "dsa/linkedlist.h"

#include <assert.h>
#include <stdio.h>

static void test_create_destroy(void)
{
    LinkedList *list = NULL;

    assert(linkedlist_create(sizeof(int), &list) == STATUS_OK);
    assert(list != NULL);
    assert(linkedlist_size(list) == 0);
    assert(linkedlist_is_empty(list));

    linkedlist_destroy(list);
    linkedlist_destroy(NULL);
}

static void test_invalid_arguments(void)
{
    LinkedList *list = NULL;

    assert(linkedlist_create(sizeof(int), NULL) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_create(0, &list) == STATUS_INVALID_ARGUMENT);
    assert(list == NULL);
    assert(linkedlist_clear(NULL) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_size(NULL) == 0);
    assert(linkedlist_is_empty(NULL));
}

static void test_create_failure_resets_output(void)
{
    LinkedList *list = NULL;
    assert(linkedlist_create(sizeof(int), &list) == STATUS_OK);

    LinkedList *out = list;
    assert(linkedlist_create(0, &out) == STATUS_INVALID_ARGUMENT);
    assert(out == NULL);

    linkedlist_destroy(list);
}

static void test_clear_empty(void)
{
    LinkedList *list = NULL;
    assert(linkedlist_create(sizeof(int), &list) == STATUS_OK);

    assert(linkedlist_clear(list) == STATUS_OK);
    assert(linkedlist_size(list) == 0);
    assert(linkedlist_is_empty(list));

    assert(linkedlist_clear(list) == STATUS_OK);
    assert(linkedlist_size(list) == 0);
    assert(linkedlist_is_empty(list));

    linkedlist_destroy(list);
}

static void test_add_invalid_arguments(void)
{
    LinkedList *list = NULL;
    int value = 10;

    assert(linkedlist_add(NULL, &value) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_add(NULL, NULL) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_create(sizeof(int), &list) == STATUS_OK);

    assert(linkedlist_add(list, NULL) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_size(list) == 0);
    assert(linkedlist_is_empty(list));

    assert(linkedlist_add(list, &value) == STATUS_OK);
    assert(linkedlist_add(list, NULL) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_size(list) == 1);
    assert(!linkedlist_is_empty(list));

    linkedlist_destroy(list);
}

static void test_add(void)
{
    LinkedList *list = NULL;
    assert(linkedlist_create(sizeof(int), &list) == STATUS_OK);

    for (int i = 0; i < 10; i++) {
        assert(linkedlist_add(list, &i) == STATUS_OK);
        assert(linkedlist_size(list) == (size_t)i + 1);
        assert(!linkedlist_is_empty(list));
    }

    linkedlist_destroy(list);
}

static void test_clear_and_reuse(void)
{
    LinkedList *list = NULL;
    assert(linkedlist_create(sizeof(int), &list) == STATUS_OK);

    for (int i = 0; i < 5; i++) {
        assert(linkedlist_add(list, &i) == STATUS_OK);
    }

    assert(linkedlist_clear(list) == STATUS_OK);
    assert(linkedlist_size(list) == 0);
    assert(linkedlist_is_empty(list));
    assert(linkedlist_clear(list) == STATUS_OK);

    int value = 99;
    assert(linkedlist_add(list, &value) == STATUS_OK);
    assert(linkedlist_size(list) == 1);
    assert(!linkedlist_is_empty(list));

    linkedlist_destroy(list);
}


static LinkedList *make_list(const int *values, size_t count)
{
    LinkedList *list = NULL;
    assert(linkedlist_create(sizeof(int), &list) == STATUS_OK);
    for (size_t i = 0; i < count; i++) {
        assert(linkedlist_add(list, &values[i]) == STATUS_OK);
    }
    return list;
}

static void assert_values(const LinkedList *list, const int *values, size_t count)
{
    assert(linkedlist_size(list) == count);
    assert(linkedlist_is_empty(list) == (count == 0));
    for (size_t i = 0; i < count; i++) {
        int actual = 0;
        assert(linkedlist_get(list, i, &actual) == STATUS_OK);
        assert(actual == values[i]);
    }
    int out = 12345;
    assert(linkedlist_get(list, count, &out) == STATUS_OUT_OF_BOUNDS);
    assert(out == 12345);
}

static void test_remaining_invalid_arguments(void)
{
    LinkedList *list = make_list(NULL, 0);
    int value = 10;
    assert(linkedlist_add(NULL, &value) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_add(list, NULL) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_get(NULL, 0, &value) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_get(list, 0, NULL) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_set(NULL, 0, &value) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_set(list, 0, NULL) == STATUS_INVALID_ARGUMENT);
    assert(linkedlist_remove(NULL, 0) == STATUS_INVALID_ARGUMENT);
    assert_values(list, NULL, 0);
    linkedlist_destroy(list);
}

static void test_add_order_and_copy(void)
{
    LinkedList *list = make_list(NULL, 0);
    int value = 20;
    assert(linkedlist_add(list, &value) == STATUS_OK);
    value = 10;
    assert(linkedlist_add(list, &value) == STATUS_OK);
    value = 30;
    assert(linkedlist_add(list, &value) == STATUS_OK);
    value = 999;
    int expected[] = {20, 10, 30};
    assert_values(list, expected, 3);
    assert(linkedlist_clear(list) == STATUS_OK);
    assert(linkedlist_add(list, &value) == STATUS_OK);
    assert_values(list, &value, 1);
    linkedlist_destroy(list);
}

static void test_get_set_and_bounds(void)
{
    LinkedList *list = make_list(NULL, 0);
    int value = 42;
    int out = -1;
    assert(linkedlist_get(list, 0, &out) == STATUS_OUT_OF_BOUNDS);
    assert(out == -1);
    assert(linkedlist_set(list, 0, &value) == STATUS_OUT_OF_BOUNDS);
    assert(linkedlist_remove(list, 0) == STATUS_OUT_OF_BOUNDS);
    assert(linkedlist_add(list, &value) == STATUS_OK);
    value = 100;
    assert(linkedlist_set(list, 0, &value) == STATUS_OK);
    value = 999;
    int expected = 100;
    assert_values(list, &expected, 1);
    assert(linkedlist_get(list, SIZE_MAX, &out) == STATUS_OUT_OF_BOUNDS);
    assert(linkedlist_set(list, 1, &value) == STATUS_OUT_OF_BOUNDS);
    assert(linkedlist_remove(list, SIZE_MAX) == STATUS_OUT_OF_BOUNDS);
    assert_values(list, &expected, 1);
    linkedlist_destroy(list);
}

static void test_remove(void)
{
    int input[] = {10, 20, 30, 40, 50};
    LinkedList *list = make_list(input, 5);
    assert(linkedlist_remove(list, 2) == STATUS_OK);
    int middle_removed[] = {10, 20, 40, 50};
    assert_values(list, middle_removed, 4);
    assert(linkedlist_remove(list, 0) == STATUS_OK);
    assert_values(list, middle_removed + 1, 3);
    assert(linkedlist_remove(list, 2) == STATUS_OK);
    int remaining[] = {20, 40};
    assert_values(list, remaining, 2);
    assert(linkedlist_remove(list, 1) == STATUS_OK);
    assert(linkedlist_remove(list, 0) == STATUS_OK);
    assert_values(list, NULL, 0);
    int value = 99;
    assert(linkedlist_add(list, &value) == STATUS_OK);
    assert_values(list, &value, 1);
    linkedlist_destroy(list);
}

static void test_append_after_tail_removal(void)
{
    int input[] = {10, 20, 30};
    LinkedList *list = make_list(input, 3);
    assert(linkedlist_remove(list, 2) == STATUS_OK);
    int value = 40;
    assert(linkedlist_add(list, &value) == STATUS_OK);
    int expected[] = {10, 20, 40};
    assert_values(list, expected, 3);

    assert(linkedlist_remove(list, 0) == STATUS_OK);
    value = 50;
    assert(linkedlist_add(list, &value) == STATUS_OK);
    int after_head_removal[] = {20, 40, 50};
    assert_values(list, after_head_removal, 3);

    assert(linkedlist_clear(list) == STATUS_OK);
    assert(linkedlist_add(list, &value) == STATUS_OK);
    value = 60;
    assert(linkedlist_add(list, &value) == STATUS_OK);
    int after_clear[] = {50, 60};
    assert_values(list, after_clear, 2);
    linkedlist_destroy(list);
}

static void test_struct_copy(void)
{
    typedef struct {
        int id;
        double value;
    } Item;
    LinkedList *list = NULL;
    assert(linkedlist_create(sizeof(Item), &list) == STATUS_OK);
    Item input = {5, 3.14};
    Item output = {0};
    assert(linkedlist_add(list, &input) == STATUS_OK);
    input.id = 99;
    input.value = 0;
    assert(linkedlist_get(list, 0, &output) == STATUS_OK);
    assert(output.id == 5 && output.value == 3.14);
    assert(linkedlist_set(list, 0, &input) == STATUS_OK);
    input.id = -1;
    assert(linkedlist_get(list, 0, &output) == STATUS_OK);
    assert(output.id == 99 && output.value == 0);
    linkedlist_destroy(list);
}

int main(void)
{
    test_create_destroy();
    test_invalid_arguments();
    test_create_failure_resets_output();
    test_clear_empty();
    test_add_invalid_arguments();
    test_add();
    test_clear_and_reuse();

    test_remaining_invalid_arguments();
    test_add_order_and_copy();
    test_get_set_and_bounds();
    test_remove();
    test_append_after_tail_removal();
    test_struct_copy();

    puts("All LinkedList tests passed!");
    return 0;
}
