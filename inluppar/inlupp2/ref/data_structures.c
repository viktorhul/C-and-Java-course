/*
 * GENERIC DATA STRUCTURES
 * Lists, hashmaps and more--if you need them.
*/

#include "data_structures.h"
#include "../inlupp1/hash_table.h"
#include "../inlupp1/linked_list.h"
#include "../inlupp1/iterator.h"

void example_func2(void) {
    return;
}

typedef struct link link_t;

struct link
{
    elem_t value;
    struct link *next;
};

struct list
{
    ioopm_eq_function elem_equiv;
    link_t *head;
    link_t *tail;
    size_t size;
};

