#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "linked_list.h"
#include "iterator.h"
#include "common.h"

/**
 * @file simple_linked_list.c
 * @author Elias Castegren
 * @date 1 Sep 2021
 * @brief A simple linked list that implements parts of the interface
 * of `linked_list.h`. The functions that are not implemented will fail
 * at runtime with an assertion error, printing a message to stderr.
 */

static link_t *link_create(elem_t value, link_t *next) {
    link_t *link = calloc(1, sizeof(link_t));
    link->value = value;
    link->next = next;
    return link;
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function func) {
    ioopm_list_t *list = calloc(1, sizeof(struct list));
    list->func = func;
    return list;
}

void ioopm_linked_list_destroy(ioopm_list_t *list) {
    assert(list);
    link_t *current = list->head;
    while (current) {
        link_t *tmp = current;
        current = current->next;
        free(tmp);
    }
    free(list);
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value) {
    if (list->size == 0) {
        list->head = link_create(value, NULL);
    } else {
        link_t *current = list->head;

        while (NULL != current->next)
            current = current->next;

        current->next = link_create(value, NULL);
    }

    list->size++;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value) {
    assert(list);
    list->head = link_create(value, list->head);
    list->size++;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value) {
    if (index == 0) {
        
        link_t *new_head = link_create(value, list->head);
        list->head = new_head;
        list->size++;
    } else if (index > 0 && index <= list->size) {
        link_t *prev = list->head;
        link_t *current = prev->next;
        for (int i = 1; i < index; i++) {
            prev = current;
            current = current->next;
        }

        current = link_create(value, current);
        prev->next = current;
        list->size++;
    }
}


elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index) {
    assert(list);
    assert(list->head);

    if (index == 0) {
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
    for (int i = 1; i < index; i++) {
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

elem_t ioopm_linked_list_get(ioopm_list_t *list, int index) {
    assert(list);
    assert(list->head);
    link_t *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
        assert(current);
    }

    return current->value;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element) {
    assert(list);
    link_t *cursor = list->head;
    ioopm_eq_function compare = list->func;

    while (cursor) {
        if (compare(cursor->value, element))
            return true;

        cursor = cursor->next;
    }

    return false;
}

size_t ioopm_linked_list_size(ioopm_list_t *list) {
    assert(list);
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list) {
    return (ioopm_linked_list_size(list) == 0);
}

static void linked_item_clear(ioopm_list_t *list) {
    while (list->size != 0)
        ioopm_linked_list_remove(list, 0);
}

void ioopm_linked_list_clear(ioopm_list_t *list) {
    linked_item_clear(list);
    list->head = NULL;
    list->size = 0;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate prop, void *extra) {
    if (list->size == 0)
        return false;

    link_t *link = list->head;
    elem_t a;

    while (NULL != link) {
        if (!prop(link->value, a, extra))
            return false;

        link = link->next;
    }

    return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate prop, void *extra) {
    if (list->size == 0)
        return false;

    link_t *link = list->head;
    elem_t a;

    while (NULL != link) {
        if (prop(link->value, a, extra))
            return true;

        link = link->next;
    }

    return false;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_function fun, void *extra) {
    link_t *link = list->head;

    while (NULL != link) {
        fun(int_elem(list->size), &link->value, link);
        link = link->next;
    }
}