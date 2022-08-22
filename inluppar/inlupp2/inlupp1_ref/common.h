#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef union elem {
    int integer;
    unsigned int unsigned_int;
    bool boolean;
    float floating;
    void *pointer;
} elem_t;

typedef unsigned long(*ioopm_hash_function)(const elem_t key);
typedef bool(*ioopm_eq_function)(elem_t key, elem_t value, elem_t extra);
