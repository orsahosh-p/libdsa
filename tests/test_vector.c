
#include "dsa/vector.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static void test_create_destroy(void)
{
    Vector *vector = NULL;

    assert(vector_create(sizeof(int), &vector) == STATUS_OK);
    assert(vector != NULL);

    assert(vector_size(vector) == 0);
    assert(vector_capacity(vector) == 0);
    assert(vector_is_empty(vector));

    vector_destroy(vector);
    vector_destroy(NULL);
}

static void test_invalid_arguments(void)
{
    Vector *vector = NULL;
    int value = 10;
    int out = 0;

    assert(vector_create(sizeof(int), NULL) == STATUS_INVALID_ARGUMENT);
    assert(vector_create(0, &vector) == STATUS_INVALID_ARGUMENT);
    assert(vector == NULL);

    assert(vector_clear(NULL) == STATUS_INVALID_ARGUMENT);
    assert(vector_reserve(NULL, 4) == STATUS_INVALID_ARGUMENT);
    assert(vector_add(NULL, &value) == STATUS_INVALID_ARGUMENT);
    assert(vector_remove(NULL, 0) == STATUS_INVALID_ARGUMENT);
    assert(vector_get(NULL, 0, &out) == STATUS_INVALID_ARGUMENT);
    assert(vector_set(NULL, 0, &value) == STATUS_INVALID_ARGUMENT);

    assert(vector_size(NULL) == 0);
    assert(vector_capacity(NULL) == 0);
    assert(vector_is_empty(NULL));

    assert(vector_create(sizeof(int), &vector) == STATUS_OK);

    assert(vector_add(vector, NULL) == STATUS_INVALID_ARGUMENT);
    assert(vector_get(vector, 0, NULL) == STATUS_INVALID_ARGUMENT);
    assert(vector_set(vector, 0, NULL) == STATUS_INVALID_ARGUMENT);

    vector_destroy(vector);
}

static void test_add_and_growth(void)
{
    Vector *vector = NULL;
    assert(vector_create(sizeof(int), &vector) == STATUS_OK);

    for (int i = 0; i < 10; i++) {
        assert(vector_add(vector, &i) == STATUS_OK);
    }

    assert(vector_size(vector) == 10);
    assert(vector_capacity(vector) == 16);

    for (int i = 0; i < 10; i++) {
        int value = -1;

        assert(vector_get(vector, (size_t)i, &value) == STATUS_OK);
        assert(value == i);
    }

    vector_destroy(vector);
}

static void test_get_set(void)
{
    Vector *vector = NULL;
    assert(vector_create(sizeof(int), &vector) == STATUS_OK);

    int value = 42;
    assert(vector_add(vector, &value) == STATUS_OK);

    value = 100;
    assert(vector_set(vector, 0, &value) == STATUS_OK);

    int result = 0;
    assert(vector_get(vector, 0, &result) == STATUS_OK);
    assert(result == 100);

    assert(vector_get(vector, 1, &result) == STATUS_OUT_OF_BOUNDS);
    assert(vector_set(vector, 1, &value) == STATUS_OUT_OF_BOUNDS);

    assert(vector_size(vector) == 1);

    vector_destroy(vector);
}

static void test_remove(void)
{
    Vector *vector = NULL;
    assert(vector_create(sizeof(int), &vector) == STATUS_OK);

    int values[] = {10, 20, 30, 40, 50};

    for (size_t i = 0; i < 5; i++) {
        assert(vector_add(vector, &values[i]) == STATUS_OK);
    }

    size_t capacity = vector_capacity(vector);

    assert(vector_remove(vector, 2) == STATUS_OK);

    int expected[] = {10, 20, 40, 50};

    for (size_t i = 0; i < 4; i++) {
        int result = 0;
        assert(vector_get(vector, i, &result) == STATUS_OK);
        assert(result == expected[i]);
    }

    assert(vector_remove(vector, 0) == STATUS_OK);
    assert(vector_remove(vector, 2) == STATUS_OK);

    assert(vector_size(vector) == 2);
    assert(vector_capacity(vector) == capacity);

    int first = 0;
    int last = 0;
    assert(vector_get(vector, 0, &first) == STATUS_OK);
    assert(vector_get(vector, 1, &last) == STATUS_OK);
    assert(first == 20 && last == 40);

    assert(vector_remove(vector, 1) == STATUS_OK);
    assert(vector_remove(vector, 0) == STATUS_OK);

    assert(vector_is_empty(vector));
    assert(vector_remove(vector, 0) == STATUS_OUT_OF_BOUNDS);

    vector_destroy(vector);
}

static void test_reserve_clear(void)
{
    Vector *vector = NULL;
    assert(vector_create(sizeof(int), &vector) == STATUS_OK);

    assert(vector_reserve(vector, 20) == STATUS_OK);
    assert(vector_capacity(vector) == 20);

    int value = 7;
    assert(vector_add(vector, &value) == STATUS_OK);

    assert(vector_reserve(vector, 5) == STATUS_OK);
    assert(vector_capacity(vector) == 20);

    assert(vector_clear(vector) == STATUS_OK);
    assert(vector_size(vector) == 0);
    assert(vector_capacity(vector) == 20);
    assert(vector_is_empty(vector));

    int next = 99;
    int result = 0;

    assert(vector_add(vector, &next) == STATUS_OK);
    assert(vector_get(vector, 0, &result) == STATUS_OK);
    assert(result == 99);

    assert(vector_reserve(vector, SIZE_MAX) == STATUS_OUT_OF_MEMORY);
    assert(vector_size(vector) == 1);
    assert(vector_capacity(vector) == 20);

    vector_destroy(vector);
}

static void test_struct_elements(void)
{
    typedef struct {
        int id;
        double value;
    } Item;

    Vector *vector = NULL;
    assert(vector_create(sizeof(Item), &vector) == STATUS_OK);

    Item input = {5, 3.14};
    Item output = {0};

    assert(vector_add(vector, &input) == STATUS_OK);
    assert(vector_get(vector, 0, &output) == STATUS_OK);

    assert(output.id == 5);
    assert(output.value == 3.14);

    vector_destroy(vector);
}

int main(void)
{
    test_create_destroy();
    test_invalid_arguments();
    test_add_and_growth();
    test_get_set();
    test_remove();
    test_reserve_clear();
    test_struct_elements();

    puts("All Vector tests passed!");
    return 0;
}
