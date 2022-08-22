#include "common.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/**
 * @file linked_list.c
 * @author Emrik Åberg Wenthzel & Patricia Müller
 * @date 18 Oct 2021
 * @brief A linked list that implements parts of the interface 
 * of linked_list.h. The functions that are not implemented will fail
 * at runtime with an assertion error, printing a message to stderr.
 */



/**
 * @file simple_linked_list.c
 * @author Elias Castegren
 * @date 1 Sep 2021
 * @brief A simple linked list that implements parts of the interface 
 * of `linked_list.h`. The functions that are not implemented will fail
 * at runtime with an assertion error, printing a message to stderr.
 */

// This is a macro that is used in the functions that are not implemented,
// to have them fail at runtime. You do not have to understand it to use
// this file!
#define NOT_SUPPORTED()                                                            \
    do                                                                             \
    {                                                                              \
        fprintf(stderr, "The simple linked list does not support %s\n", __func__); \
        assert(false);                                                             \
    } while (0)

// The links of the linked list
struct link
{
    elem_t value;
    struct link *next;
};

typedef struct link link_t;
typedef bool(*ioopm_int_predicate)(elem_t value, elem_t extra);
typedef bool(*ioopm_apply_int_function)(elem_t value, elem_t extra);

static link_t *link_create(elem_t value, link_t *next)
{
    link_t *link = calloc(1, sizeof(link_t));
    link->value = value;
    link->next = next;
    return link;
}

// The list contains a pointer to its first link, and its size
struct list
{
    ioopm_eq_function elem_equiv;
    link_t *head;
    link_t *tail;
    size_t size;
};



ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function elem_equiv)
{
    ioopm_list_t *result = calloc(1, sizeof(struct list));
    if (result)
    {
        result->head = result->tail = calloc(1, sizeof(struct list));
    }
    result->elem_equiv = elem_equiv;
    result->size = 0;
    return result;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    assert(list);
    link_t *current = list->head;
    while (current)
    {
        link_t *tmp = current;
        current = current->next;
        free(tmp);
    }
    free(list);
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
    assert(list);
    if (&(list->head) == &(list->tail))
    {
        ioopm_linked_list_prepend(list, value);
    }
    else
    {
        link_t *link = link_create(value, NULL);
        list->tail->next = link;
        list->tail = link;
    }
    list->size++;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    assert(list);
    link_t *link = link_create(value, NULL);
    if (&(list->head) == &(list->tail))
    {
        list->tail = link;
    }
    else
    {
        link->next = list->head->next;
    }
    list->head->next = link;
    list->size++;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{
    assert(list);
    assert(list->head);
    if (index == 0)
    {
        ioopm_linked_list_prepend(list, value);
    }
    else
    {
        link_t *prev = list->head;
        for (int i = 1; i < index; i++)
        {
            prev = prev->next;
        }
        link_t *new_link = link_create(value, prev->next);
        prev->next = new_link;
    }
    list->size++;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
    assert(list);
    assert(list->head);

    if (index == 0)
    {
        link_t *tmp = list->head;
        elem_t value = tmp->value;
        list->head = tmp->next;
        free(tmp);
        list->size--;
        return value;
    }

    // index > 0
    assert(list->head);
    link_t *prev = list->head;
    link_t *current = prev->next;
    for (int i = 1; i < index; i++)
    {
        assert(current);
        prev = current;
        current = current->next;
    }

    prev->next = current->next;
    elem_t value = current->value;
    free(current);
    list->size--;
    return value;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
    assert(list);
    assert(list->head);
    link_t *current = list->head;
    for (int i = 0; i < index; i++)
    {
        if (current->next == NULL)
            return (elem_t) { .pointer = NULL };

        current = current->next;
        //assert(current);
    }
    return current->value;
}

char **ioopm_linked_list_to_string_array(ioopm_list_t *list)
{
    assert(list);
    assert(list->head);
    size_t size = ioopm_linked_list_size(list);
    char **keys = calloc(size, sizeof(char*));
    link_t *current = list->head;
    for (int i = 0; i < size; i++)
    {
        current = current->next;
        keys[i] = current->value.pointer;
        assert(current);
    }
    return keys;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
    assert(list);
    link_t *cursor = list->head;
    while (cursor)
    {
        if(list->elem_equiv(cursor->value, element, element)) // OBS! olika ordning på key_equivs argument i testing.c och linked_list.c, vilken ska vi behålla? 
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    assert(list);
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return !(ioopm_linked_list_size(list)); // return TRUE if empty 
}


void ioopm_linked_list_clear(ioopm_list_t *list)
{
    assert(list);
    for (int i = list->size; 0<=i; i--)
    {
        ioopm_linked_list_remove(list, i);
    } 
    free(list);
}


bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate prop, elem_t extra)
{
    assert(list);
    bool result = true;
    link_t *pointer_to_current = list->head;
    while (pointer_to_current->next != NULL && result)
    {
        result = result && prop(pointer_to_current->value, extra);
        pointer_to_current = pointer_to_current->next;
    }
    return result;
}


bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate prop, elem_t extra)
{
    assert(list);
    bool result = false;
    link_t *pointer_to_current = list->head;
    while (pointer_to_current->next != NULL)
    {
      result = result || prop(pointer_to_current->next->value, extra);
      pointer_to_current = pointer_to_current->next;
    }
    
    return result;
}


void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_linked_function fun, elem_t extra)
{
    assert(list);
    link_t *pointer_to_current = list->head;
    while (pointer_to_current->next != NULL)
    {
      fun(pointer_to_current->next->value, extra);
      pointer_to_current = pointer_to_current->next;
    }
}
