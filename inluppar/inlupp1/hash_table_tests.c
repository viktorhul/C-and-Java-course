#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"

static size_t next_prime(size_t index) {
    size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381, 30763};
    return primes[index];
}

// Help functions to test ioopm_hash_table_any och _all

static bool string_eq(elem_t e1, elem_t e2) {
    return (strcmp(e1.p, e2.p) == 0);
}

static void ioopm_hash_table_replace_to_wow(elem_t key, elem_t *value, void *str) {
    elem_t new_value;
    new_value = ptr_elem("wow");
    *value = new_value;
}

static bool modulo(elem_t key, elem_t value, void *x) {
    return (key.i % 17 == 1);
}

void test_insert(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(1);
    elem_t item_value1 = ptr_elem("test");
    elem_t item_value2 = ptr_elem("test2");

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key1, item_value2);


    CU_ASSERT_EQUAL(item_value2.p, table->buckets[item_key1.i % table->bucket_size]->next->value.p);
    ioopm_hash_table_destroy(table);
}   

void test_insert_negative_key(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key = int_elem(-5);
    elem_t item_value1 = ptr_elem("test");

    size_t hash = table->hash_function(item_key);
    size_t bucket = abs(hash % table->bucket_size);

    ioopm_hash_table_insert(table, item_key, item_value1);
    
    CU_ASSERT_EQUAL(table->buckets[bucket]->next->key.i, item_key.i);

    //CU_ASSERT_TRUE(result.success);
    
    ioopm_hash_table_destroy(table);
}

void test_insert_two_elements_different_buckets(void) {
     ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(1);
    elem_t item_key2 = int_elem(3);
    elem_t item_value1 = ptr_elem("test");
    elem_t item_value2 = ptr_elem("test2");

    option_t result1 = ioopm_hash_table_insert(table, item_key1 , item_value1);
    option_t result2 = ioopm_hash_table_insert(table, item_key2 , item_value2);

    CU_ASSERT_EQUAL(item_value1.p, result1.value.p);
    CU_ASSERT_EQUAL(item_value2.p, result2.value.p);

    CU_ASSERT_EQUAL(item_value1.p, table->buckets[item_key1.i % table->bucket_size]->next->value.p);
    CU_ASSERT_EQUAL(item_value2.p, table->buckets[item_key2.i % table->bucket_size]->next->value.p);

    ioopm_hash_table_destroy(table);    
}

void test_insert_two_elements_same_bucket(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(1);
    elem_t item_key2 = int_elem(1 + table->bucket_size);
    elem_t item_value1 = ptr_elem("test");
    elem_t item_value2 = ptr_elem("test2");

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);

    CU_ASSERT_EQUAL(item_value1.p, table->buckets[item_key1.i % table->bucket_size]->next->value.p);
    CU_ASSERT_EQUAL(item_value2.p, table->buckets[item_key2.i % table->bucket_size]->next->next->value.p);
    
    ioopm_hash_table_destroy(table);
}

void test_table_all(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(1);
    elem_t item_key2 = int_elem(1 + table->bucket_size);
    elem_t item_value1 = ptr_elem("test");
    elem_t item_value2 = ptr_elem("test2");

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);

    char *a = NULL;
    bool is_true = ioopm_hash_table_all(table, modulo, a);

    CU_ASSERT_TRUE(is_true);
    ioopm_hash_table_destroy(table);
}


void test_apply_to_all(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(5 + table->bucket_size);
    elem_t item_value1 = ptr_elem("test");
    elem_t item_value2 = ptr_elem("test2");

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);

    ioopm_hash_table_apply_to_all(table, ioopm_hash_table_replace_to_wow, NULL);

    elem_t first_value = table->buckets[5 % table->bucket_size]->next->value;

    CU_ASSERT(0 == strcmp(first_value.p, "wow"));
    CU_ASSERT(0 != strcmp(first_value.p, item_value1.p));
    
    ioopm_hash_table_destroy(table);
}

void test_empty_hash_table_has_key(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    bool has_key = ioopm_hash_table_has_key(table, int_elem(5));
    
    CU_ASSERT_FALSE(has_key);

    ioopm_hash_table_destroy(table);
}

void test_several_entries_hash_table_has_key(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(5 + table->bucket_size);
    elem_t item_value1 = ptr_elem("test");
    elem_t item_value2 = ptr_elem("test2");

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);

    bool has_item_key1 = ioopm_hash_table_has_key(table, item_key1);
    bool has_item_key2 = ioopm_hash_table_has_key(table, item_key2);

    CU_ASSERT_TRUE(has_item_key1);
    CU_ASSERT_TRUE(has_item_key2);

    ioopm_hash_table_destroy(table);
}

void test_empty_hash_table_has_value(void){
     ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
     bool has_value = ioopm_hash_table_has_value(table, ptr_elem("test"));
     CU_ASSERT_FALSE(has_value);
     ioopm_hash_table_destroy(table);
}

void test_several_entries_hash_table_has_value(void){
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(5 + table->bucket_size);
    elem_t item_value1 = ptr_elem("test");
    elem_t item_value2 = ptr_elem("test2");

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);
    
    bool has_item_value1 = ioopm_hash_table_has_value(table, item_value1);
    bool has_item_value2 = ioopm_hash_table_has_value(table, item_value2);
    CU_ASSERT_TRUE(has_item_value1);
    CU_ASSERT_TRUE(has_item_value2);
    ioopm_hash_table_destroy(table);
}

void test_several_entries_duplicate_hash_table_has_value(void){
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(5 + table->bucket_size);
    elem_t item_value1 = ptr_elem("test");
    elem_t item_value2 = ptr_elem("test2");
    
    char *item_value1_dup = strdup(item_value1.p);
    elem_t item_value1_dup_elem = ptr_elem(item_value1_dup);

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);
    
    bool has_item_value1 = ioopm_hash_table_has_value(table, item_value1_dup_elem);
    CU_ASSERT_TRUE(has_item_value1);

    free(item_value1_dup);
    ioopm_hash_table_destroy(table);
}

void test_hash_table_keys(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(5 + table->bucket_size);
    elem_t item_value1 = ptr_elem("test");
    elem_t item_value2 = ptr_elem("test2");

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);

    ioopm_list_t *result = ioopm_hash_table_keys(table);

    CU_ASSERT_TRUE(ioopm_hash_table_has_key(table, item_key1));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(table, item_key2));

    ioopm_linked_list_destroy(result);
    ioopm_hash_table_destroy(table);
}

void test_hash_table_values(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(5 + table->bucket_size);
    elem_t item_value1 = ptr_elem("test");
    elem_t item_value2 = ptr_elem("test2");

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);

    ioopm_list_t *result = ioopm_hash_table_values(table);

    CU_ASSERT(result->head->value.p == item_value2.p);
    CU_ASSERT(result->head->next->value.p == item_value1.p);

    ioopm_linked_list_destroy(result);
    ioopm_hash_table_destroy(table);
}

void test_empty_hash_table_keys(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    ioopm_list_t *result = ioopm_hash_table_keys(table);

    CU_ASSERT_EQUAL(0, result->size);

    ioopm_linked_list_destroy(result);
    ioopm_hash_table_destroy(table);
}

void test_empty_hash_table_values(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    ioopm_list_t *result = ioopm_hash_table_values(table);

    CU_ASSERT(ioopm_linked_list_is_empty(result));

    ioopm_linked_list_destroy(result);
    ioopm_hash_table_destroy(table);
}

void test_size_empty_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    size_t table_size = ioopm_hash_table_size(table);

    CU_ASSERT(0 == table_size);

    ioopm_hash_table_destroy(table);
}

void test_size_singular_entry_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key = int_elem(5);
    elem_t item_value = ptr_elem("test string");

    ioopm_hash_table_insert(table, item_key, item_value);

    size_t table_size = ioopm_hash_table_size(table);

    CU_ASSERT(1 == table_size);

    ioopm_hash_table_destroy(table);
}

void test_size_several_entries_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(7);
    elem_t item_value1 = ptr_elem("test string");
    elem_t item_value2 = ptr_elem("test string2");

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);

    size_t table_size = ioopm_hash_table_size(table);

    CU_ASSERT(2 == table_size);

    ioopm_hash_table_destroy(table);
}

void test_if_empty_empty_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    bool answer = ioopm_hash_table_is_empty(table);
    CU_ASSERT_TRUE(answer);
    
    ioopm_hash_table_destroy(table);

}

void test_if_empty_single_entry_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    
    elem_t item_key = int_elem(5);
    elem_t item_value = ptr_elem("test string");
    ioopm_hash_table_insert(table, item_key, item_value);
    
    bool answer = ioopm_hash_table_is_empty(table);

    CU_ASSERT_FALSE(answer);
    
    ioopm_hash_table_destroy(table);

}

void test_if_empty_several_entries_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    
    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(6);
    elem_t item_value1 = ptr_elem("test string");
    elem_t item_value2 = ptr_elem("test string");
    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);
    
    bool answer = ioopm_hash_table_is_empty(table);

    CU_ASSERT_FALSE(answer);
    
    ioopm_hash_table_destroy(table);

}

void test_if_clear_empty_hash_table(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    
    ioopm_hash_table_clear(table);
    bool answer = ioopm_hash_table_is_empty(table);

    CU_ASSERT_TRUE(answer);
    
    ioopm_hash_table_destroy(table);
}

void test_if_clear_single_entry_hash_table(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    
    elem_t item_key = int_elem(5);
    elem_t item_value = ptr_elem("test string");
    ioopm_hash_table_insert(table, item_key, item_value);
    
    ioopm_hash_table_clear(table);

    bool answer = ioopm_hash_table_is_empty(table);

    CU_ASSERT_TRUE(answer);
    
    ioopm_hash_table_destroy(table);
}

void test_if_clear_several_entries_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    
    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(6);
    elem_t item_value1 = ptr_elem("test string");
    elem_t item_value2 = ptr_elem("test string2");
    
    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);
    
    ioopm_hash_table_clear(table);
    bool answer = ioopm_hash_table_is_empty(table);

    CU_ASSERT_TRUE(answer);
    
    ioopm_hash_table_destroy(table);
}

void test_add_item_to_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);

    elem_t item_key = int_elem(5);
    elem_t item_value = ptr_elem("test string");

    ioopm_hash_table_insert(table, item_key, item_value);
    
    CU_ASSERT_EQUAL(table->buckets[item_key.i % table->bucket_size]->next->key.i, item_key.i);
    CU_ASSERT_EQUAL(table->buckets[item_key.i % table->bucket_size]->next->value.p, item_value.p);

    ioopm_hash_table_destroy(table);
}

void test_add_two_items_to_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    elem_t item_key = int_elem(5);
    elem_t item_value1 = ptr_elem("test string1");
    elem_t item_value2 = ptr_elem("test string2");
    ioopm_hash_table_insert(table, item_key, item_value1);
    ioopm_hash_table_insert(table, item_key, item_value2);
    CU_ASSERT_EQUAL(table->buckets[item_key.i % table->bucket_size]->next->key.i, item_key.i);
    CU_ASSERT_EQUAL(table->buckets[item_key.i % table->bucket_size]->next->value.p, item_value2.p);
    ioopm_hash_table_destroy(table);
}

void test_add_two_different_items_to_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(5 + table->bucket_size);
    elem_t item_value1 = ptr_elem("test string1");
    elem_t item_value2 = ptr_elem("test string2");

    ioopm_hash_table_insert(table, item_key1, item_value1);
    ioopm_hash_table_insert(table, item_key2, item_value2);

    CU_ASSERT_EQUAL(table->buckets[item_key1.i % table->bucket_size]->next->key.i, item_key1.i);
    CU_ASSERT_EQUAL(table->buckets[item_key1.i % table->bucket_size]->next->value.p, item_value1.p);

    CU_ASSERT_EQUAL(table->buckets[item_key1.i % table->bucket_size]->next->next->key.i, item_key2.i);
    CU_ASSERT_EQUAL(table->buckets[item_key1.i % table->bucket_size]->next->next->value.p, item_value2.p);

    ioopm_hash_table_destroy(table);
}

void test_ioopm_hash_table_lookup(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    elem_t item_key = int_elem(5);
    elem_t item_value = ptr_elem("test string1");

    option_t result = ioopm_hash_table_lookup(table, item_key);
    CU_ASSERT_FALSE(Successful(result));
    ioopm_hash_table_insert(table, item_key, item_value);
    result = ioopm_hash_table_lookup(table, item_key);
    CU_ASSERT(Successful(result));


    CU_ASSERT(result.success);
    CU_ASSERT_TRUE(NULL != result.value.p);

    ioopm_hash_table_destroy(table);
}

void test_lookup_empty_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    elem_t item_key = int_elem(5);

    option_t result = ioopm_hash_table_lookup(table, item_key);

    CU_ASSERT_TRUE(Unsuccessful(result));
    CU_ASSERT_FALSE(result.success);

    ioopm_hash_table_destroy(table);
}

void test_lookup_several_items_one_bucket(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(5 + table->bucket_size);
    elem_t item_string1 = ptr_elem("test");
    elem_t item_string2 = ptr_elem("test2");

    ioopm_hash_table_insert(table, item_key1, item_string1);
    ioopm_hash_table_insert(table, item_key2, item_string2);

    option_t result1 = ioopm_hash_table_lookup(table, item_key1);
    CU_ASSERT_TRUE(Successful(result1));
    CU_ASSERT_EQUAL(item_string1.p, result1.value.p)

    option_t result2 = ioopm_hash_table_lookup(table, item_key2);
    CU_ASSERT_TRUE(Successful(result2));
    CU_ASSERT_EQUAL(item_string2.p, result2.value.p)

    ioopm_hash_table_destroy(table);
}

void test_remove_key_empty_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    option_t result = ioopm_hash_table_remove(table, int_elem(5));

    CU_ASSERT_TRUE(Unsuccessful(result));

    ioopm_hash_table_destroy(table);
}

void test_remove_key_singular_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    elem_t item_key = int_elem(5);
    elem_t item_string = ptr_elem("test");
    ioopm_hash_table_insert(table, item_key, item_string);
    option_t result = ioopm_hash_table_remove(table, item_key);
    CU_ASSERT_TRUE(Successful(result));

    ioopm_hash_table_destroy(table);
}

void test_remove_key_several_hash_table(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    elem_t item_key1 = int_elem(5);
    elem_t item_key2 = int_elem(5 + table->bucket_size);
    elem_t item_string1 = ptr_elem("abc");
    elem_t item_string2 = ptr_elem("def");
    ioopm_hash_table_insert(table, item_key1, item_string1);
    ioopm_hash_table_insert(table, item_key2, item_string2);

    option_t result = ioopm_hash_table_remove(table, item_key1);

    CU_ASSERT_TRUE(Successful(result));

    ioopm_hash_table_destroy(table);
}

void test_remove_key_non_existent(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    elem_t item_key = int_elem(5);
    elem_t item_string = ptr_elem("test");
    ioopm_hash_table_insert(table, item_key, item_string);
    option_t result = ioopm_hash_table_remove(table, int_elem(8));
    CU_ASSERT_TRUE(Unsuccessful(result));

    ioopm_hash_table_destroy(table);
}

void test_remove_negative_key(void)
{
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, string_eq);
    elem_t item_key = int_elem(5);
    elem_t item_string = ptr_elem("test");
    ioopm_hash_table_insert(table, item_key, item_string);
    option_t result = ioopm_hash_table_remove(table, int_elem(-8));
    CU_ASSERT_TRUE(Unsuccessful(result));

    ioopm_hash_table_destroy(table);
}

void test_dynamic_buckets_new_dummies(void) {
    ioopm_hash_table_t *table = ioopm_hash_table_create(NULL, NULL, NULL);
    bool all_buckets_found = true;

    for (int i = 0; i < next_prime(0); i++) {
        if (table->buckets[i] == NULL)
            all_buckets_found = false;
    }
    
    CU_ASSERT_TRUE(all_buckets_found);
    all_buckets_found = true;

    size_t threshold = next_prime(0) * table->load_factor + 1;

    for (int i = 0; i < threshold; i++) {
        ioopm_hash_table_insert(table, int_elem(i), ptr_elem("test string"));
    }

    for (int i = 0; i < next_prime(1); i++) {
        if (table->buckets[i] == NULL)
            all_buckets_found = false;
    }

    CU_ASSERT_TRUE(all_buckets_found);

    ioopm_hash_table_destroy(table);

}

int main(void)
{
    CU_pSuite test_suite1 = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    test_suite1 = CU_add_suite("Hash tables", 0, 0);
    if (NULL == test_suite1)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        NULL == CU_add_test(test_suite1, "Insert two elements with the same key and the same bucket", test_insert) ||
        NULL == CU_add_test(test_suite1, "Insert negative key", test_insert_negative_key) ||
        NULL == CU_add_test(test_suite1, "Insert two elements in different buckets", test_insert_two_elements_different_buckets) ||
        NULL == CU_add_test(test_suite1, "Insert two elements in the same bucket", test_insert_two_elements_same_bucket) ||
        NULL == CU_add_test(test_suite1, "Add one item to bucket", test_add_item_to_hash_table) ||
        NULL == CU_add_test(test_suite1, "Add two iis  an item in an empty table", test_lookup_empty_hash_table) ||
        NULL == CU_add_test(test_suite1, "Lookup several items in table", test_lookup_several_items_one_bucket) ||
        NULL == CU_add_test(test_suite1, "Tim found lookup test", test_ioopm_hash_table_lookup) ||
        NULL == CU_add_test(test_suite1, "Remove key from empty hash table", test_remove_key_empty_hash_table) ||
        NULL == CU_add_test(test_suite1, "Remove key from singular hash table", test_remove_key_singular_hash_table) ||
        NULL == CU_add_test(test_suite1, "Remove key from a hashtable with several keys", test_remove_key_several_hash_table) ||
        NULL == CU_add_test(test_suite1, "Remove non-existent key", test_remove_key_non_existent) ||
        NULL == CU_add_test(test_suite1, "Remove negative key", test_remove_negative_key) ||
        NULL == CU_add_test(test_suite1, "Size of table with zero entries", test_size_empty_hash_table) ||
        NULL == CU_add_test(test_suite1, "Size of table with one entry", test_size_singular_entry_hash_table) ||
        NULL == CU_add_test(test_suite1, "Size of table with two entries different buckets", test_size_several_entries_hash_table) ||
        NULL == CU_add_test(test_suite1, "If empty empty hash table", test_if_empty_empty_hash_table) ||
        NULL == CU_add_test(test_suite1, "If empty one entry", test_if_empty_single_entry_hash_table) ||
        NULL == CU_add_test(test_suite1, "If empty several entries different buckets", test_if_empty_several_entries_hash_table) ||
        NULL == CU_add_test(test_suite1, "If clear empty hash table", test_if_clear_empty_hash_table) ||
        NULL == CU_add_test(test_suite1, "If clear one entry", test_if_clear_single_entry_hash_table) ||
        NULL == CU_add_test(test_suite1, "If clear several entries", test_if_clear_several_entries_hash_table) ||
        NULL == CU_add_test(test_suite1, "Find all keys in hash table", test_hash_table_keys) ||
        NULL == CU_add_test(test_suite1, "Find all values in hash table", test_hash_table_values) ||
        NULL == CU_add_test(test_suite1, "Keys of empty hash table", test_empty_hash_table_keys) ||
        NULL == CU_add_test(test_suite1, "Values of empty hash table", test_empty_hash_table_values) ||
        NULL == CU_add_test(test_suite1, "Key found in empty hash table", test_empty_hash_table_has_key) ||
        NULL == CU_add_test(test_suite1, "Key found in hash table with several entries", test_several_entries_hash_table_has_key) ||
        NULL == CU_add_test(test_suite1, "Value found in empty hash table", test_empty_hash_table_has_value) ||
        NULL == CU_add_test(test_suite1, "Value found in hash table with several entries", test_several_entries_hash_table_has_value) ||
        NULL == CU_add_test(test_suite1, "Value equals duplicate in hash table with several entries", test_several_entries_duplicate_hash_table_has_value) ||
        NULL == CU_add_test(test_suite1, "Test apply all change value", test_apply_to_all) ||
        NULL == CU_add_test(test_suite1, "All table keys belong to the condition", test_table_all) ||
        NULL == CU_add_test(test_suite1, "Resize hash table with dynamic bucket size", test_dynamic_buckets_new_dummies)
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