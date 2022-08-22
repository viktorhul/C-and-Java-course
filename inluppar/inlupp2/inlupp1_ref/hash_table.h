#pragma once
#include "common.h"
#include <stdlib.h>
#include "linked_list.h"

/**
 * @file hash_table.h
 * @author Emrik Åberg Wenthzel & Patricia Müller
 * @date 18 Oct 2021
 * @brief A hash table that maps keys to values.
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gf5efa1610dfd73b58fef071f6c1d7a90
 */

typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;
typedef bool(*ioopm_predicate)(elem_t key, elem_t value, elem_t arg1);
typedef void(*ioopm_apply_function)(elem_t key, elem_t value, elem_t extra);


/// @brief Create a new hash table
/// @param hash_function a function that returns a hash code mapped to by key 
/// @param comparekey a fuction that compares keys
/// @param comparevalue a fuction that compares values
/// @param total_buckets the size of all allocated buckets  
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_function, ioopm_eq_function comparekey, ioopm_eq_function comparevalue, size_t total_buckets);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief update key in an existing value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert_inc_value_if_exists(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return the element mapped to by key
elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the element mapped to by key
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

/// @brief returns the number of key => value entries in the hash table
/// @param ht hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param ht hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param ht hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param ht hash table operated upon
/// @return an array of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param ht hash table operated upon
/// @return an array of values for hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param ht hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param ht hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg1);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg1);

/// @brief apply a function to all entries in a hash table
/// @param ht hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, elem_t arg);

/// @brief if the loadfactor is reached, makes a new hash table with the optimal prime as buckets (maximum 16381), frees the memory if the old table if load factor is reached
/// @param ht hash table to increase
/// @return new hash table with same elements and updated bucket size
ioopm_hash_table_t *ioopm_hash_table_load_factor_check(ioopm_hash_table_t *ht);

/// @brief prints the buckets and elements of hash table ht
/// @param ht hash table to be printed
/// @return the size of the total elements in ht
size_t ioopm_hash_table_print(ioopm_hash_table_t *ht);