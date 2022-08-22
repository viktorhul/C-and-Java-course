#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "iterator.h"
#include "linked_list.h"
#include "common.h"

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list) {
    ioopm_list_iterator_t *iterator = calloc(1, sizeof(ioopm_list_iterator_t));
        
    iterator->current = list->head;
    iterator->list = list;

    return iterator;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter) {
    if (iter->current == NULL) {
        return false;
    }
    return iter->current->next != NULL;
}

option_t ioopm_iterator_next(ioopm_list_iterator_t *iter) {
    if (ioopm_iterator_has_next(iter)) {
        iter->current = iter->current->next;
        return Success(iter->current->value);
    } else {
        return Failure();
    }
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter) {
    iter->current = iter->list->head;
}

option_t ioopm_iterator_current(ioopm_list_iterator_t *iter) {
    if (iter->current != NULL) {
        return Success(iter->current->value);
    } else {
        return Failure();
    }
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter) {
    free(iter);
    iter = NULL;
}