#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash_table.h"
#include <string.h>
#include "linked_list.h"
#include "common.h"
#include "iterator.h"

static size_t next_prime(size_t index) {
    size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381, 30763};
    return primes[index];
}

bool string_eq(elem_t e1, elem_t e2) {
    return (strcmp(e1.p, e2.p) == 0);
}

static bool int_eq(elem_t a, elem_t b) {
    return (a.i == b.i);
}

int extract_int_hash_key(elem_t key) {
    return key.i;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) {
    entry_t *ptr;

    for (int i = 0; i < ht->bucket_size; i++)
    {
        ptr = ht->buckets[i]->next;

        while (NULL != ptr)
        {
            if (!pred(ptr->key, ptr->value, arg))
                return false;
            ptr = ptr->next;
        }
    }

    return true;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) {
    entry_t *ptr;

    for (int i = 0; i < ht->bucket_size; i++) {
        ptr = ht->buckets[i]->next;

        while (NULL != ptr) {
            if (pred(ptr->key, ptr->value, arg))
                return true;

            ptr = ptr->next;
        }
    }

    return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg) {
    entry_t *ptr;

    for (int i = 0; i < ht->bucket_size; i++) {
        ptr = ht->buckets[i]->next;

        while (NULL != ptr) {
            apply_fun(ptr->key, &ptr->value, ptr);

            ptr = ptr->next;
        }
    }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key) {
    ioopm_hash_function hash_func = ht->hash_function;
    size_t bucket = abs(hash_func(key) % ht->bucket_size);
    entry_t *ptr = ht->buckets[bucket]->next;

    while (ptr != NULL) {
        if (ht->key_eq_function(ptr->key, key))
            return true;
        ptr = ptr->next;
    }

    return false;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value) {
    if (ioopm_hash_table_is_empty(ht))
        return false;

    ioopm_eq_function compare = ht->value_eq_function;
    ioopm_list_t *list = ioopm_hash_table_values(ht);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

    do {
        if (compare(ioopm_iterator_current(iter).value, value)) {
            ioopm_linked_list_destroy(list);
            ioopm_iterator_destroy(iter);
            return true;
        }
    } while (ioopm_iterator_next(iter).success);

    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iter);

    return false;
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht) {
    size_t array_size = ioopm_hash_table_size(ht);
    ioopm_list_t *list = ioopm_linked_list_create(NULL);

    if (0 == array_size)
        return list;

    for (int i = 0; i < ht->bucket_size; i++) {
        entry_t *ptr = ht->buckets[i]->next;

        while (NULL != ptr) {
            ioopm_linked_list_prepend(list, ptr->key);
            ptr = ptr->next;
        }
    }

    return list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht) {
    size_t array_size = ioopm_hash_table_size(ht);
    ioopm_list_t *list = ioopm_linked_list_create(NULL);

    if (0 == array_size)
        return list;

    for (int i = 0; i < ht->bucket_size; i++) {
        entry_t *ptr = ht->buckets[i]->next;

        while (NULL != ptr) {
            ioopm_linked_list_prepend(list, ptr->value);
            ptr = ptr->next;
        }
    }

    return list;
}

static void entry_destroy(entry_t *entry) {
    free(entry);
    entry = NULL;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht) {
    if (ioopm_hash_table_is_empty(ht))
        return;

    for (int i = 0; i < ht->bucket_size; i++) {
        entry_t *ptr = ht->buckets[i]->next;
        entry_t *ptr_next;

        while (NULL != ptr) {
            ptr_next = ptr->next;
            entry_destroy(ptr);
            ptr = ptr_next;
        }

        ht->buckets[i]->next = NULL;
    }
    ht->entry_size = 0;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht) {
    return ioopm_hash_table_size(ht) == 0;
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht) {
    return ht->entry_size;
    size_t counter = 0;

    for (int i = 0; i < ht->bucket_size; i++) {
        entry_t *ptr = ht->buckets[i]->next;

        while (NULL != ptr) {
            counter++;
            ptr = ptr->next;
        }
    }
    return counter;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    for (int i = 0; i < ht->bucket_size; i++) {
        entry_t *ptr = ht->buckets[i];
        entry_t *ptr_next = ptr->next;

        while (NULL != ptr) {
            ptr_next = ptr->next;
            entry_destroy(ptr);
            ptr = ptr_next;
        }
    }

    free(ht->buckets);
    free(ht);
    ht = NULL;
}

static entry_t *find_previous_entry_for_key(entry_t *bucket, elem_t key, ioopm_eq_function key_compare) {
    while (1) {
        if (bucket->next == NULL || key_compare(bucket->next->key, key))
            return bucket;

        bucket = bucket->next;
    }
}

option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key) {
    int hash = ht->hash_function(key);

    option_t entry = ioopm_hash_table_lookup(ht, key);

    if (Unsuccessful(entry))
        return Failure();

    entry_t *previous = find_previous_entry_for_key(ht->buckets[abs(hash % ht->bucket_size)], key, ht->key_eq_function);
    entry_t *current = previous->next;
    elem_t current_value = current->value;
    previous->next = current->next;
    entry_destroy(current);
    return Success(current_value);
}

option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key) {
    ioopm_eq_function func = ht->key_eq_function;
    ioopm_hash_function hash_func = ht->hash_function;
    int hash = hash_func(key);

    entry_t *previous = find_previous_entry_for_key(ht->buckets[abs(hash % ht->bucket_size)], key, func);
    entry_t *current = previous->next;

    if (current) {
        return Success(current->value);
    } else {
        return Failure();
    }
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next) {
    entry_t *entry = calloc(1, sizeof(entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = next;

    return entry;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function function, ioopm_eq_function key_eq_function, ioopm_eq_function value_eq_function) {
    ioopm_hash_table_t *table = calloc(1, sizeof(ioopm_hash_table_t));
    int prime = next_prime(table->prime_index);
    table->prime_index = 0;
    table->load_factor = 0.75;
    table->bucket_size = next_prime(0);
    table->buckets = calloc(1,prime * sizeof(entry_t *));

    for (int i = 0; i < table->bucket_size; ++i)
        table->buckets[i] = entry_create(int_elem(0), ptr_elem(NULL), NULL);

    table->hash_function = (NULL != function) ? function : extract_int_hash_key;
    table->key_eq_function = (NULL != key_eq_function) ? key_eq_function : int_eq;
    table->value_eq_function = (NULL != value_eq_function) ? value_eq_function : string_eq;

    return table;
}

ioopm_hash_table_t *rehash(ioopm_hash_table_t *ht) {
    size_t size = ioopm_hash_table_size(ht);
    size = size + 1;
    double load = (double) size / ht->bucket_size;
    if (!(load >= ht->load_factor)) {
        return ht;
    } else {
        ht->prime_index++;
        ioopm_list_t *keys = ioopm_hash_table_keys(ht);
        ioopm_list_t *values = ioopm_hash_table_values(ht);
        ioopm_list_iterator_t *keys_iter = ioopm_list_iterator(keys);
        ioopm_list_iterator_t *values_iter = ioopm_list_iterator(values);
        ioopm_hash_table_clear(ht);
        ht->buckets = realloc(ht->buckets, next_prime(ht->prime_index) * sizeof(entry_t *));
        
        for (int i = ht->bucket_size; i < next_prime(ht->prime_index); i++) {
            ht->buckets[i] = entry_create(int_elem(0), ptr_elem(NULL), NULL);
        }
        ht->bucket_size = next_prime(ht->prime_index);

        while (1) {
            ioopm_hash_table_insert(ht, ioopm_iterator_current(keys_iter).value, ioopm_iterator_current(values_iter).value);

            if (!ioopm_iterator_has_next(keys_iter))
                break;

            ioopm_iterator_next(keys_iter);
            ioopm_iterator_next(values_iter);
        }

        ioopm_iterator_destroy(keys_iter);
        ioopm_iterator_destroy(values_iter);
        ioopm_linked_list_destroy(keys);
        ioopm_linked_list_destroy(values);

        return ht;
    }
}

option_t ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) {
    ioopm_hash_table_t *table = rehash(ht);
    *ht = *table;

    size_t hash = table->hash_function(key);
    size_t bucket = abs(hash % table->bucket_size);

    entry_t *entry = find_previous_entry_for_key(table->buckets[bucket], key, table->key_eq_function);
    entry_t *next = entry->next;
    if (next != NULL && table->key_eq_function(next->key, key)) {
        next->value = value;
        return Success(next->value);
    } else {
        ht->entry_size++;
        entry->next = entry_create(key, value, next);
        return Success(entry->next->value);
    }
}