#include "common.h"
#include "iterator.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct iter ioopm_list_iterator_t; 
typedef struct link link_t; 
typedef struct list list_t; 

struct link
{
    elem_t value;
    struct link *next;

};

struct list
{
    ioopm_eq_function compare;
    link_t *head;
    link_t *tail;
    size_t size;
};

struct iter 
{
  link_t *current;
  ioopm_list_t *list; /// New field
};

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list){
    ioopm_list_iterator_t *result = calloc(1, sizeof(ioopm_list_iterator_t));
    result->current = list->head;
    result->list = list;
    return result;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    return iter->current->next != NULL;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
    iter->current = iter->current->next;
    return iter->current->value;
}

elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
    link_t *to_remove = iter->current->next; 
    elem_t result = to_remove->value;

    iter->current->next = to_remove->next; 

    free(to_remove); 

    return result;
}

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element)
{
    link_t *to_append = iter->current;

    iter->current->value = element;
    iter->current->next = to_append;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    iter->current = iter->list->head;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    return iter->current->value;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter) {
    free(iter);
    iter = NULL;
}
