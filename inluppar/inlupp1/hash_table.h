#pragma once
#include <stdbool.h>
#include "linked_list.h"
#include "common.h"

/**
 * @file hash_table.h
 * @author Niklas Gotowiec, Viktor Hultsten
 * @date 14 September 2021
 * @brief Simple hash table that maps integer keys to string values. 
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gf5efa1610dfd73b58fef071f6c1d7a90
 */

typedef struct entry entry_t;
struct entry
{
  elem_t key;       // holds the key
  elem_t value;     // holds the value
  entry_t *next;    // points to the next entry (possibly NULL)
};

typedef struct hash_table ioopm_hash_table_t;
struct hash_table
{
  entry_t **buckets;                    // An array of buckets in the hash table, dynamic, the first time you create a table theres 17 buckets.
  size_t entry_size;                    // How many entries are in the table, the size.
  size_t bucket_size;                   // The ammount of buckets in the hash table.
  size_t prime_index;                   // Variable to keep track of which index to pick in the primes array that we use to determine the ammount of buckets when resizing the hash table.
  double load_factor;                   //Load factor of the table.
  ioopm_hash_function hash_function;    //Hash function to use for hashing keys, if it's NULL we will use our own default hashing function.
  ioopm_eq_function key_eq_function;    //Key equality function used for checking if two keys are equivalent, if it's NULL we will use our own default comparing function.
  ioopm_eq_function value_eq_function;  // Value equality function used for checking if two values are equivalent, if it's NULL we will use our own default comparing function.
};


/// @brief Create a new hash table
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function function, ioopm_eq_function key_eq_function, ioopm_eq_function value_eq_function);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
option_t ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return the value mapped to by key
option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key
option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param ht hash table operated upon
/// @return a linked list of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return an array of values for hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg);