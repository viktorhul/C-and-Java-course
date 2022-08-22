#pragma once

#include "inlupp1_ref/hash_table.h"
#include "inlupp1_ref/linked_list.h"
#include "inlupp1_ref/iterator.h"

/**
 * @brief A structure representing three hash tables
 * 1. merch_items, all merch in store
 * 2. shelves, all shelves including names of merchandise 
 * 3. shopping_carts existing shopping carts in store database
 */
typedef struct database database_t;

/**
 * @brief A structure representing a merchandise
 * Contains name, description, total quantity available in store and a list of storage locations
 */
typedef struct merch merch_t;

/**
 * @brief A structure representing storage locations
 * Contains a list of all storage with belonging merch 
 */
typedef struct shelf shelf_t;

/**
 * @brief A structure representing an item in a shopping cart
 * Contains name of the item and quantities 
 */
typedef struct cart_item cart_item_t;


#define ptr_elem(x) (elem_t) { .pointer = x }

#define OPTION_FORMAT_CHARS(x) (valid_format_t) { .options = ptr_elem(x) }
#define OPTION_FORMAT_RANGE(a, b) (valid_format_t) { .range = { .min = a, .max = b } }
#define OPTION_FORMAT_TEXT() (valid_format_t) {0}

#define INPUT_CHARS 1
#define INPUT_INT 2
#define INPUT_TEXT 3
#define INPUT_SHELF 4

typedef union valid_format valid_format_t;
union valid_format {
    elem_t options;
    struct {
        int min;
        int max;
    } range;
};

struct database {
    elem_t username;
    ioopm_hash_table_t *merch_items;
    ioopm_hash_table_t *shelves;
    ioopm_hash_table_t *shopping_carts;
};

struct merch {
    elem_t name;
    elem_t desc;
    int price;
    int total_quantity;
    ioopm_list_t *locations;
};

struct shelf {
    elem_t shelf;
    int quantity;
};

struct cart_item {
    elem_t merch;
    int quantity;
};