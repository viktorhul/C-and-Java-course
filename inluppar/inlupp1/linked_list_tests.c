#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>
#include "linked_list.h"
#include "iterator.h"
#include "common.h"

// Help functions

static bool int_greater_than(elem_t a, elem_t b, void *extra) {
    int *c = extra;
    return a.i > *c;
}

static void add_one_to_key(elem_t a, elem_t *b, void *extra) {
    link_t *link = extra;
    link->value = int_elem((*b).i + 1);
}

static bool equal_int(elem_t a, elem_t b) {
    return a.i == b.i;
}

void test_ioopm_linked_list_create_and_destroy(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    CU_ASSERT(0 == list->size);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_append(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    
    elem_t list2_value = ioopm_linked_list_get(list, list->size-1);

    CU_ASSERT(2 == list2_value.i);

    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_prepend(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_linked_list_prepend(list,int_elem(2));
    ioopm_linked_list_prepend(list,int_elem(3));
    CU_ASSERT(3 == list->head->value.i);
    
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_insert_invalid(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_linked_list_insert(list,1,int_elem(3));
    ioopm_linked_list_insert(list,-1,int_elem(8));
    CU_ASSERT(list->size == 0);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_insert_valid(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);

    size_t size_before = list->size;

    ioopm_linked_list_insert(list, 0, int_elem(1));
    ioopm_linked_list_insert(list, 1, int_elem(2));
    ioopm_linked_list_insert(list, 2, int_elem(3));
    ioopm_linked_list_insert(list, 2, int_elem(6));

    size_t size_after = list->size;
    elem_t third_value = ioopm_linked_list_get(list, 2);
    elem_t fourth_value = ioopm_linked_list_get(list, 3);
    
    CU_ASSERT(size_before + 4 == size_after);
    CU_ASSERT(third_value.i == 6);
    CU_ASSERT(fourth_value.i == 3);

    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_remove(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_list_t *list2 = ioopm_linked_list_create(equal_int);

    size_t size_before = list->size;

    ioopm_linked_list_insert(list, 0, int_elem(1));
    ioopm_linked_list_insert(list, 1, int_elem(2));
    ioopm_linked_list_insert(list, 2, int_elem(3));
    ioopm_linked_list_remove(list, 1);

    size_t size_after = list->size;
    elem_t list_value = ioopm_linked_list_get(list, 1);
    
    CU_ASSERT(size_before + 2 == size_after);
    CU_ASSERT(list_value.i == 3);

    elem_t nothing_to_remove = ioopm_linked_list_remove(list2,5);
    CU_ASSERT_PTR_NULL(nothing_to_remove.p);

    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_get(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);

    ioopm_linked_list_insert(list, 0, int_elem(1));
    ioopm_linked_list_insert(list, 1, int_elem(2));
    ioopm_linked_list_insert(list, 2, int_elem(3));
    
    elem_t list_value = ioopm_linked_list_get(list, 2);
    
    CU_ASSERT(list_value.i == 3);

    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_contains(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);

    ioopm_linked_list_insert(list, 0, int_elem(1));
    ioopm_linked_list_insert(list, 1, int_elem(2));
    ioopm_linked_list_insert(list, 2, int_elem(3));

    CU_ASSERT_TRUE(ioopm_linked_list_contains(list,int_elem(1)));

    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_size(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);

    ioopm_linked_list_insert(list, 0, int_elem(1));
    ioopm_linked_list_insert(list, 1, int_elem(2));

    size_t size = list->size;
    
    CU_ASSERT(size == 2);

    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_clear(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);

    ioopm_linked_list_insert(list, 0, int_elem(1));
    ioopm_linked_list_insert(list, 1, int_elem(2));
    ioopm_linked_list_insert(list, 2, int_elem(3));
    ioopm_linked_list_insert(list, 3, int_elem(4));

    ioopm_linked_list_clear(list);

    size_t size = list->size;
    
    CU_ASSERT(size == 0);

    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_all(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);

    ioopm_linked_list_insert(list, 0, int_elem(2));
    ioopm_linked_list_insert(list, 1, int_elem(3));
    ioopm_linked_list_insert(list, 2, int_elem(4));
    ioopm_linked_list_insert(list, 3, int_elem(5));
    ioopm_linked_list_insert(list, 4, int_elem(6));

    int n = 5;
    int m = 1;
    bool all_greater_than_five = ioopm_linked_list_all(list, int_greater_than, &n);
    bool all_greater_than_one = ioopm_linked_list_all(list, int_greater_than, &m);
    
    CU_ASSERT_FALSE(all_greater_than_five);
    CU_ASSERT_TRUE(all_greater_than_one);

    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_any(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);

    ioopm_linked_list_insert(list, 0, int_elem(2));
    ioopm_linked_list_insert(list, 1, int_elem(3));
    ioopm_linked_list_insert(list, 2, int_elem(4));
    ioopm_linked_list_insert(list, 3, int_elem(5));
    ioopm_linked_list_insert(list, 4, int_elem(6));

    int n = 5;
    int m = 10;
    bool any_greater_than_five = ioopm_linked_list_any(list, int_greater_than, &n);
    bool any_greater_than_ten = ioopm_linked_list_any(list, int_greater_than, &m);
    
    CU_ASSERT_TRUE(any_greater_than_five);
    CU_ASSERT_FALSE(any_greater_than_ten);

    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_apply_to_all(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);

    ioopm_linked_list_insert(list, 0, int_elem(2));
    ioopm_linked_list_insert(list, 1, int_elem(3));
    ioopm_linked_list_insert(list, 2, int_elem(4));
    ioopm_linked_list_insert(list, 3, int_elem(5));
    ioopm_linked_list_insert(list, 4, int_elem(6));

    int n = 2;
    bool all_greater_than_two = ioopm_linked_list_all(list, int_greater_than, &n);
    CU_ASSERT_FALSE(all_greater_than_two);

    char *a;
    ioopm_linked_list_apply_to_all(list, add_one_to_key, a);

    all_greater_than_two = ioopm_linked_list_all(list, int_greater_than, &n);
    CU_ASSERT_TRUE(all_greater_than_two);

    ioopm_linked_list_destroy(list);
}

void test_iterator_has_next_one_remaining(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_linked_list_insert(list, 0, int_elem(2));
    ioopm_linked_list_insert(list, 1, int_elem(3));
    ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);

    bool has_next = ioopm_iterator_has_next(iterator);

    CU_ASSERT_TRUE(has_next);

    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_iterator_has_next_no_remaining(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_linked_list_insert(list, 0, int_elem(2));
    ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);

    bool has_next = ioopm_iterator_has_next(iterator);

    CU_ASSERT_FALSE(has_next);

    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_iterator_next_one_remaining(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_linked_list_insert(list, 0, int_elem(2));
    ioopm_linked_list_insert(list, 1, int_elem(3));
    ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);

    option_t next = ioopm_iterator_next(iterator);

    CU_ASSERT_TRUE(next.success);
    CU_ASSERT_EQUAL(next.value.i, 3);

    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_iterator_next_no_remaining(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_linked_list_insert(list, 0, int_elem(2));
    ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);

    option_t next = ioopm_iterator_next(iterator);

    CU_ASSERT_FALSE(next.success);
    CU_ASSERT_EQUAL(next.value.i, NULL);

    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_iterator_reset(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_linked_list_insert(list, 0, int_elem(2));
    ioopm_linked_list_insert(list, 1, int_elem(3));
    ioopm_linked_list_insert(list, 2, int_elem(6));
    ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);
    
    ioopm_iterator_next(iterator);
    option_t current = ioopm_iterator_next(iterator);
    CU_ASSERT_EQUAL(current.value.i, 6);
    
    ioopm_iterator_reset(iterator);
    current = ioopm_iterator_current(iterator);
    CU_ASSERT_EQUAL(current.value.i, 2);

    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_iterator_current_empty_list(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);

    option_t current = ioopm_iterator_current(iterator);

    CU_ASSERT_FALSE(current.success);
    CU_ASSERT_EQUAL(current.value.i, NULL);

    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_iterator_current_nonempty_list(void) {
    ioopm_list_t *list = ioopm_linked_list_create(equal_int);
    ioopm_linked_list_insert(list, 0, int_elem(2));
    ioopm_linked_list_insert(list, 1, int_elem(3));
    ioopm_linked_list_insert(list, 2, int_elem(6));
    ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);

    option_t current = ioopm_iterator_current(iterator);
    
    CU_ASSERT_TRUE(current.success);
    CU_ASSERT_EQUAL(current.value.i, 2);
    
    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

int main(void)
{
    CU_pSuite test_suite1 = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    test_suite1 = CU_add_suite("Linked list", 0, 0);
    if (NULL == test_suite1)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        NULL == CU_add_test(test_suite1, "Create and destroy a list", test_ioopm_linked_list_create_and_destroy) ||
        NULL == CU_add_test(test_suite1, "Append two items into list", test_ioopm_linked_list_append) ||
        NULL == CU_add_test(test_suite1, "Prepend two items into list", test_ioopm_linked_list_prepend) ||
        NULL == CU_add_test(test_suite1, "Insert invalid index into list", test_ioopm_linked_list_insert_invalid) ||
        NULL == CU_add_test(test_suite1, "Insert valid index into list", test_ioopm_linked_list_insert_valid) ||
        NULL == CU_add_test(test_suite1, "Remove an entry from the list", test_ioopm_linked_list_remove) ||
        NULL == CU_add_test(test_suite1, "Get an entry from the list", test_ioopm_linked_list_get) ||
        NULL == CU_add_test(test_suite1, "Check if the list contains an entry with value x", test_ioopm_linked_list_contains) ||
        NULL == CU_add_test(test_suite1, "Check size of list", test_ioopm_linked_list_size) ||
        NULL == CU_add_test(test_suite1, "Remove all elements in list", test_ioopm_linked_list_clear) ||
        NULL == CU_add_test(test_suite1, "Check if condition in all elements hold", test_ioopm_linked_list_all) ||
        NULL == CU_add_test(test_suite1, "Check if condition in at least one element holds", test_ioopm_linked_list_any) ||
        NULL == CU_add_test(test_suite1, "Apply function to all keys in list", test_ioopm_linked_apply_to_all) ||
        NULL == CU_add_test(test_suite1, "Iterator if next value in list", test_iterator_has_next_one_remaining) ||
        NULL == CU_add_test(test_suite1, "Iterator if next value in list with no remaining elements", test_iterator_has_next_no_remaining) ||
        NULL == CU_add_test(test_suite1, "Return iterator next value in list", test_iterator_next_one_remaining) ||
        NULL == CU_add_test(test_suite1, "Return iterator next value in list with no remaining elements", test_iterator_next_no_remaining) ||
        NULL == CU_add_test(test_suite1, "Reset iterator", test_iterator_reset) ||
        NULL == CU_add_test(test_suite1, "Current iterator value of empty list", test_iterator_current_empty_list) ||
        NULL == CU_add_test(test_suite1, "Current iterator value of non-empty list", test_iterator_current_nonempty_list)
    )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}