#include "common.h"
#include "linked_list.h"
#include "hash_table.h"
#include "iterator.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * @file hash_table.c
 * @author Emrik Åberg Wenthzel & Patricia Müller
 * @date 18 Oct 2021
 * @brief A hash table that implements parts of the interface 
 * of `hash_table.h`. 
 */

typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;

struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  ioopm_hash_function hash_function;
  ioopm_eq_function compare_value;
  ioopm_eq_function compare_key;
  size_t total_buckets;
  size_t size;
  entry_t *buckets;
};

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_function, ioopm_eq_function comparekey, ioopm_eq_function comparevalue, size_t total_buckets)
{
  /// Allocate space for a ioopm_hash_table_t = No_Buckets pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result->hash_function = hash_function;
  result->compare_value = comparevalue;
  result->compare_key = comparekey;
  result->size = 0;
  result->total_buckets = total_buckets;
  result->buckets = calloc(total_buckets, sizeof(entry_t));
  return result;
}

static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, elem_t key)
{
  int bucket = (ht->hash_function(key))%(ht->total_buckets);
  entry_t *entry_coutner = &(ht->buckets[bucket]);
  while(1)
  {
    if(entry_coutner->next == NULL || ht->compare_key(entry_coutner->next->key, key, key))
    {
      return entry_coutner;
    }
    else
    {
      entry_coutner = entry_coutner->next;
    }
  }
} 

static entry_t *entry_create(elem_t key,elem_t value,entry_t *next)
{
  entry_t *t = calloc(1, sizeof(entry_t));
  t->key = key;
  t->value = value;
  t->next = next;
  return t;
}

static void entry_destroy(entry_t *entry)
{
  free(entry);
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  entry_t *tmp = find_previous_entry_for_key(ht, key);
  elem_t returnstring  = {.pointer = ""};
  entry_t *removedest = tmp->next;
  if (tmp->next == NULL)
  {
    return returnstring;
  }
  else if (tmp->next->next != NULL)
  {
    returnstring = tmp->next->value;
    tmp->next = tmp->next->next;
    entry_destroy(removedest);
    ht->size--;
    return returnstring;
  }
  else
  {
    returnstring = tmp->next->value;
    tmp->next = NULL;
    entry_destroy(removedest);
    return returnstring;
  }
}

elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
  /// Find the previous entry for key
  entry_t *tmp = find_previous_entry_for_key(ht, key);
  entry_t *next = tmp->next;
  elem_t null = {0};
  if (next && ht->compare_key(next->key,key,key))
  {
    return next->value;
  }
  else 
  {
    //printf("Error = %s. The key %d was not found\n", strerror(EINVAL), key.integer);
    return null;
  }
}

static void hash_table_insert_apply_pred(elem_t key, elem_t value, elem_t htp)
{
  ioopm_hash_table_t *ht = htp.pointer;
  entry_t *entry = find_previous_entry_for_key(ht, key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next && ht->compare_key(next->key, key, key))
  {
    next->value = value;
  }
  else
  {
    entry->next = entry_create(key, value, next);
    ht->size++;
  }
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  /// Calculate the bucket for this entry
  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(ht, key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next && ht->compare_key(next->key, key, key))
    {
      next->value = value;
    }
  else
    {
      entry->next = entry_create(key, value, next);
      ht->size++;
    }
}

void ioopm_hash_table_insert_inc_value_if_exists(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  /// Calculate the bucket for this entry
  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(ht, key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next && ht->compare_key(next->key, key, key))
    {
      next->value.integer++;
      free(key.pointer);
    }
  else
    {
      entry->next = entry_create(key, value, next);
      ht->size++;
    }
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  size_t bucketacc = 0;
  entry_t *pointercache; 
  entry_t *realremove; 
  while (bucketacc < ht->total_buckets)
  {
    pointercache = ht->buckets[bucketacc].next;
    
    while (pointercache != NULL)
    {
      realremove = pointercache;
      pointercache = realremove->next;
      entry_destroy(realremove);
    }
    bucketacc++;
  }
  free(ht->buckets);
  ht->size = 0;

  return;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_clear(ht);
  free(ht);
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  size_t bucketacc = 0;
  entry_t *entrypointer; 
  size_t result = 0;
  while (bucketacc < ht->total_buckets)
  {
    entrypointer = ht->buckets[bucketacc].next;
    
    while (entrypointer != NULL)
    {
      entrypointer = entrypointer->next;
      result++;
    }
    bucketacc++;
    
  }
  ht->size = result;
  return result;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_size(ht) == 0;
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *resultarray = ioopm_linked_list_create(ht->compare_key);
  entry_t *entrypointer; 
  size_t bucketacc = 0;
  int resultacc = 0;
  while (bucketacc < ht->total_buckets)
  {
    entrypointer = ht->buckets[bucketacc].next;
    
    while (entrypointer != NULL)
    {
      ioopm_linked_list_append(resultarray, entrypointer->key);
      entrypointer = entrypointer->next;
      resultacc++;
    }
    bucketacc++;
  }
  return resultarray;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht) {
  ioopm_list_t *list = ioopm_linked_list_create(ht->compare_value);
  
  for (int i = 0; i < ht->total_buckets; i++) {
    entry_t *entry = ht->buckets[i].next;

    while (entry != NULL) {
      ioopm_linked_list_append(list, entry->value);
      entry = entry->next;
    }
  }

  return list;
}

ioopm_list_t *ioopm_hash_table_values2(ioopm_hash_table_t *ht)
{
  ioopm_list_t *resultarray = ioopm_linked_list_create(ht->compare_value);
  entry_t *entrypointer; 
  size_t bucketacc = 0;
  int resultacc = 0;
  while (bucketacc < ht->total_buckets)
  {
    entrypointer = ht->buckets[bucketacc].next;
    
    while (entrypointer != NULL)
    {
      ioopm_linked_list_append(resultarray, entrypointer->value);
      entrypointer = entrypointer->next;
      resultacc++;
    }
    bucketacc++;
  }
  return resultarray;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg1)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  bool result = true;
  for (size_t i = 0; i < size && result; ++i)
  {
    result = result && pred(ioopm_linked_list_get(keys, i+1), ioopm_linked_list_get(values, i+1), arg1);
  }
  ioopm_linked_list_destroy(keys);
  ioopm_linked_list_destroy(values);
  return result;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg1)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  bool result = false;
  for (size_t i = 0; i < size; ++i)
  {
    result = result || pred(ioopm_linked_list_get(keys, i+1), ioopm_linked_list_get(values, i+1), arg1);
  }
  ioopm_linked_list_destroy(keys);
  ioopm_linked_list_destroy(values);
  return result;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, elem_t arg)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  for (size_t i = 0; i < size; ++i)
  {
    apply_fun(ioopm_linked_list_get(keys, i+1), ioopm_linked_list_get(values, i+1), arg);
  }
  ioopm_linked_list_destroy(keys);
  ioopm_linked_list_destroy(values);
  return;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  return ioopm_hash_table_any(ht, ht->compare_key, key);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  return ioopm_hash_table_any(ht, ht->compare_value, value);
}

static size_t nextprime(size_t current_buckets)
{
  size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};
  for (size_t i = 0; i < 10; i++)
  {
    if(current_buckets < primes[i])return primes[i];
  }
  return primes[10];
}

ioopm_hash_table_t *ioopm_hash_table_load_factor_check(ioopm_hash_table_t *ht)
{
  size_t prime = ht->total_buckets;
  float loadfactor = ht->size/prime;
  if(loadfactor > 0.75)
  {
    while (loadfactor > 0.75 && prime != nextprime(prime))
    {
      prime = nextprime(prime);
      loadfactor = ht->size/ht->total_buckets;
      printf("%ld", prime);
    }

    ioopm_hash_table_t *newht = ioopm_hash_table_create(ht->hash_function, ht->compare_key, ht->compare_value, prime);
    ioopm_hash_table_apply_to_all(ht, hash_table_insert_apply_pred, (elem_t){.pointer = newht});
    ioopm_hash_table_destroy(ht);
    return newht;
  }
  else
  {
    return ht;
  }
}

size_t ioopm_hash_table_print(ioopm_hash_table_t *ht)
{
  size_t bucketacc = 0;
  entry_t *entrypointer; 
  size_t result = 0;
  while (bucketacc < ht->total_buckets)
  {
    entrypointer = ht->buckets[bucketacc].next;
    printf("in bucket %ld : \n", bucketacc);
    
    while (entrypointer != NULL)
    {
      printf("        %s\n", (char*)entrypointer->key.pointer);
      entrypointer = entrypointer->next;
      result++;
    }
    bucketacc++;
    
  }
  return result;
}


