/*
 * BUSINESS LOGIC
 * All non-user interface logic that concerns the system, e.g. shopping
 * carts, merchandise, adding and removing items from the database.
*/
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "inlupp1_ref/hash_table.h"
#include "inlupp1_ref/linked_list.h"
#include "inlupp1_ref/iterator.h"

#include "business_logic.h"
#include "common.h"
#include "generic_utils.h"
#define No_buckets 17

database_t *ioopm_create_database() {
    database_t *database = calloc(1, sizeof(database_t));
    database->merch_items = ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_string_key_eq, ioopm_string_eq, No_buckets);
    database->shelves = ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_string_key_eq, ioopm_string_eq, No_buckets);
    database->shopping_carts = ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_string_key_eq, ioopm_string_eq, No_buckets);
    
    return database;
}

static void destroy_merch_items(database_t *database) {
    ioopm_list_t *merch_items = ioopm_hash_table_values(database->merch_items);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_items);

    while (ioopm_iterator_has_next(iter)) {
        merch_t *merch = ioopm_iterator_next(iter).pointer;
        free(merch->name.pointer);
        free(merch->desc.pointer);
        ioopm_linked_list_destroy(merch->locations);
        free(merch);
    }

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(merch_items);
    ioopm_hash_table_destroy(database->merch_items);
}

static void destroy_shelves(database_t *database) {
    ioopm_list_t *shelves = ioopm_hash_table_values(database->shelves);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(shelves);

    while (ioopm_iterator_has_next(iter)) {
        shelf_t *shelf = ioopm_iterator_next(iter).pointer;
        free(shelf->shelf.pointer);
        free(shelf);
    }
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(shelves);
    ioopm_hash_table_destroy(database->shelves);
}

static void destroy_shopping_carts(database_t *database) {
    assert(database);
    ioopm_list_t *shopping_carts = ioopm_hash_table_values(database->shopping_carts);
    assert(shopping_carts);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(shopping_carts);

    while (ioopm_iterator_has_next(iter)) {
        ioopm_list_t *l = ioopm_iterator_next(iter).pointer; 
        ioopm_list_iterator_t *i = ioopm_list_iterator(l);

        while (ioopm_iterator_has_next(i)) {
            cart_item_t *c = ioopm_iterator_next(i).pointer; 
            free(c);
        }
        ioopm_iterator_destroy(i);

        ioopm_linked_list_destroy(l);
    }
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(shopping_carts);
    ioopm_hash_table_destroy(database->shopping_carts);
}

void ioopm_destroy_database(database_t *database) {
    assert(database);

    destroy_merch_items(database);
    destroy_shelves(database);
    destroy_shopping_carts(database);

    free(database);
    database = NULL;
    return;
}

bool ioopm_merch_exists(database_t *database, elem_t name) {
    assert(database);
    assert(name.pointer);
    bool return_value = false;
    ioopm_list_t *merch_list = ioopm_hash_table_values(database->merch_items);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_list);
    while (ioopm_iterator_has_next(iter)) {
        merch_t *m = ioopm_iterator_next(iter).pointer;
        if (0 == strcmp(m->name.pointer, name.pointer)) {
            return_value = true;
            break;
        }
    }
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(merch_list);
    return return_value;
}

static bool empty_elem(elem_t elem) {
    assert(elem.pointer);
    bool name_is_empty = ('\0' == *((char *) elem.pointer));

    return name_is_empty;
}

bool ioopm_add_merch(database_t *database, elem_t m_name, elem_t m_desc, int price) {
    assert(database);

    if (empty_elem(m_name))
        return false;

    if (price < 0)
        return false;

    if (ioopm_merch_exists(database, m_name))
        return false;

    merch_t *merch = calloc(1, sizeof(merch_t));

    elem_t name_ptr = ptr_elem(strdup(m_name.pointer));

    merch->name = name_ptr;
    merch->desc = ptr_elem(strdup(m_desc.pointer));
    merch->price = price;
    merch->total_quantity = 0;
    merch->locations = ioopm_linked_list_create(ioopm_string_eq);
    
    ioopm_hash_table_insert(database->merch_items, name_ptr, ptr_elem(merch));
    return true;
}

static void ioopm_cleanup_shopping_cart(database_t *database, elem_t name) {
    assert(database);
    if (ioopm_cart_exists(database)) {
        ioopm_list_t *shopping_cart = ioopm_hash_table_lookup(database->shopping_carts, database->username).pointer;
        ioopm_list_iterator_t *iter = ioopm_list_iterator(shopping_cart);

        int index = 0;
        while (ioopm_iterator_has_next(iter)) {
            cart_item_t *current_cart_item = ioopm_iterator_next(iter).pointer;
            merch_t *old_merch_in_cart = current_cart_item->merch.pointer;

            if (0 == strcmp(old_merch_in_cart->name.pointer, name.pointer)) {
                free(current_cart_item);
                ioopm_linked_list_remove(shopping_cart, index);
                break;
            }

            index++;
        }
        ioopm_iterator_destroy(iter);
    }
}

bool ioopm_remove_merch(database_t *database, elem_t name) {
    assert(database);

    if (!ioopm_merch_exists(database, name))
        return false;
    
    bool return_value = false;


    ioopm_list_t *list = ioopm_hash_table_values(database->merch_items);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

    while (ioopm_iterator_has_next(iter)) {
        merch_t *merch = ioopm_iterator_next(iter).pointer;
        
        if (0 == strcmp(merch->name.pointer, name.pointer)) {
            ioopm_cleanup_shopping_cart(database, name);
            ioopm_hash_table_remove(database->merch_items, merch->name);
            ioopm_linked_list_destroy(merch->locations);
            free(merch->name.pointer);
            free(merch->desc.pointer);
            free(merch);
            return_value = true;
            break;
        }
    }

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);

    return return_value;
}



bool ioopm_edit_merch(database_t *database, elem_t name, elem_t new_name, elem_t new_desc, int new_price) {
    assert(database);

    if (!ioopm_merch_exists(database, name))
        return false;

    if (ioopm_merch_exists(database, new_name) && (0 != strcmp(name.pointer, new_name.pointer)))
        return false;

    merch_t *merch = NULL;

    ioopm_list_t *temp = ioopm_hash_table_values(database->merch_items);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(temp);
    while (ioopm_iterator_has_next(iter)) {
        merch_t *m = ioopm_iterator_next(iter).pointer;
        if ((0 == strcmp(m->name.pointer, name.pointer))) {
            merch = m;
            break;
        }
    }
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(temp);

    ioopm_cleanup_shopping_cart(database, name);

    if (0 != strcmp(merch->name.pointer, new_name.pointer)) {
        elem_t new_name_ptr = ptr_elem(strdup(new_name.pointer));
        
        ioopm_hash_table_remove(database->merch_items, merch->name);
        ioopm_hash_table_insert(database->merch_items, new_name_ptr, ptr_elem(merch));
        
        free(merch->name.pointer);
        merch->name = new_name_ptr;
    }

    if (0 != strcmp(merch->desc.pointer, new_desc.pointer)) {
        free(merch->desc.pointer);
        merch->desc = ptr_elem(strdup(new_desc.pointer));
    }

    if (merch->price != new_price) {
        merch->price = new_price;
    }

    return true;
}

bool ioopm_shelf_occupied(database_t *database, elem_t shelf) {
    assert(database);
    return ioopm_hash_table_has_key(database->shelves, shelf);
}

void remove_shelf(database_t *database, elem_t shelf_ptr) {
    shelf_t *shelf = shelf_ptr.pointer;

    ioopm_hash_table_remove(database->shelves, shelf->shelf);
    free(shelf->shelf.pointer);
    free(shelf);
}

void remove_from_shelves(database_t *database, elem_t merch_ptr, int quantity) {
    merch_t *merch = merch_ptr.pointer;
    ioopm_list_t *merch_locs = merch->locations;
    ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_locs);
    
    while (ioopm_iterator_has_next(iter) && quantity != 0) {
        shelf_t *shelf = ioopm_iterator_next(iter).pointer;

        if (shelf->quantity >= quantity) {
            shelf->quantity = shelf->quantity - quantity;
            quantity = 0;
            if (shelf->quantity == 0) {
                remove_shelf(database, ptr_elem(shelf));
                ioopm_linked_list_remove(merch_locs, 0);
            }
        } else {
            quantity = quantity - shelf->quantity;
            remove_shelf(database, ptr_elem(shelf));
            ioopm_linked_list_remove(merch_locs, 0);        
        }
    }

    ioopm_iterator_destroy(iter);
}

void ioopm_check_out(database_t *database) {
    assert(database);
    ioopm_list_t *shopping_cart = ioopm_hash_table_lookup(database->shopping_carts, database->username).pointer;
    ioopm_list_iterator_t *iter = ioopm_list_iterator(shopping_cart);
    while (ioopm_iterator_has_next(iter)) {
        cart_item_t *current_cart_item = ioopm_iterator_next(iter).pointer;
        merch_t *merch = current_cart_item->merch.pointer;
        assert(merch->total_quantity >= current_cart_item->quantity);
        merch->total_quantity = merch->total_quantity - current_cart_item->quantity;
        remove_from_shelves(database, ptr_elem(merch), current_cart_item->quantity);
        free(current_cart_item);
    }
    ioopm_linked_list_destroy(shopping_cart);
    ioopm_hash_table_remove(database->shopping_carts, database->username);
    ioopm_iterator_destroy(iter);
}

static bool valid_shelf_format(elem_t shelf) {
    char *s = shelf.pointer;
    
    bool valid_format = (
        isalpha(s[0]) &&
        isdigit(s[1]) &&
        isdigit(s[2]) &&
        '\0' == s[3]
    );
    
    return valid_format;
}

bool ioopm_replenish(database_t *database, elem_t name, elem_t shelf_name, int units) {
    assert(database);
    
    if (units < 0){ 
        return false;
    }

    if (empty_elem(name) || empty_elem(shelf_name)){ 
        return false;
    }

    if (!ioopm_merch_exists(database, name)){ 
        return false;
    }

    if (ioopm_shelf_occupied(database, shelf_name)){ 
        return false;
    }

    if (!valid_shelf_format(shelf_name)){ 
        return false;
    }

    if (ioopm_shelf_occupied(database, shelf_name)) {
        merch_t *merch = NULL;

        ioopm_list_t *temp = ioopm_hash_table_values(database->merch_items);
        ioopm_list_iterator_t *iter = ioopm_list_iterator(temp);
        while (ioopm_iterator_has_next(iter)) {
            merch_t *m = ioopm_iterator_next(iter).pointer;
            if ((0 == strcmp(m->name.pointer, name.pointer))) {
                merch = m;
                break;
            }
        }
        ioopm_iterator_destroy(iter);
        ioopm_linked_list_destroy(temp);
        merch_t *m = merch;
        ioopm_list_t *merch_shelves = m->locations;
        ioopm_list_iterator_t *merch_iter = ioopm_list_iterator(merch_shelves);
        
        while (ioopm_iterator_has_next(merch_iter)) {
            shelf_t *s = ioopm_iterator_next(merch_iter).pointer;
            if (0 == strcmp(s->shelf.pointer, shelf_name.pointer)) {
                m->total_quantity = m->total_quantity + units;
                s->quantity = s->quantity + units;
                ioopm_iterator_destroy(merch_iter);
                return true;
            }
        }
        ioopm_iterator_destroy(merch_iter);
        return false;
    }

    elem_t shelf_name_ptr = ptr_elem(strdup(shelf_name.pointer));

    shelf_t *shelf = calloc(1, sizeof(shelf_t));
    shelf->shelf = shelf_name_ptr;
    shelf->quantity = units;

    merch_t *merch = NULL;

    ioopm_list_t *temp = ioopm_hash_table_values(database->merch_items);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(temp);
    while (ioopm_iterator_has_next(iter)) {
        merch_t *m = ioopm_iterator_next(iter).pointer;
        if ((0 == strcmp(m->name.pointer, name.pointer))) {
            merch = m;
            break;
        }
    }
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(temp);

    merch->total_quantity = merch->total_quantity + units;

    ioopm_linked_list_prepend(merch->locations, ptr_elem(shelf));
    ioopm_hash_table_insert(database->shelves, shelf_name_ptr, ptr_elem(shelf));

    return true;
}

ioopm_list_t *ioopm_show_stock(merch_t *name) {
    return name->locations;
    ioopm_list_t *key_locations = name->locations;
    ioopm_list_t *all_keys = ioopm_linked_list_create(ioopm_string_eq);

    size_t size = ioopm_linked_list_size(key_locations);
    char *keys[size];
    for (size_t i = 0; i < size; i++)
    {
        elem_t key = ioopm_linked_list_get(key_locations, i+1);
        keys[i] = key.pointer;
    }

    for (size_t i = 0; i < size && i < 20; i++) {
        ioopm_linked_list_append(all_keys, ptr_elem(keys[i]));
    }
    ioopm_linked_list_destroy(key_locations);

}

bool ioopm_cart_is_empty(ioopm_list_t *shopping_cart) {
    return ioopm_linked_list_is_empty(shopping_cart);
} 

bool ioopm_cart_exists(database_t *database) {
    assert(database);
    return ioopm_hash_table_has_key(database->shopping_carts, database->username);
}

void ioopm_create_shopping_cart(database_t *database) {
    assert(database);
    ioopm_list_t *shopping_cart = ioopm_linked_list_create(ioopm_string_eq);
    ioopm_hash_table_insert(database->shopping_carts, database->username, ptr_elem(shopping_cart)); 
}


merch_t *find_merch(database_t *database, elem_t merch_name) {

    ioopm_list_t *merch_list = ioopm_hash_table_keys(database->merch_items);
    ioopm_list_iterator_t *iterm = ioopm_list_iterator(merch_list);
    merch_t *merch = NULL;

    while (ioopm_iterator_has_next(iterm)) {
        elem_t m_name = ioopm_iterator_next(iterm);

        if (0 == strcmp(merch_name.pointer, m_name.pointer)) {
            merch = ioopm_hash_table_lookup(database->merch_items, m_name).pointer;
            break;
        }
    }
    ioopm_iterator_destroy(iterm);
    ioopm_linked_list_destroy(merch_list);
    return merch;
}

bool ioopm_add_to_cart(database_t *database, elem_t merch_name, int quantity) {
    assert(database);
    if (quantity <= 0)
        return false;

    if (!ioopm_cart_exists(database)) {
        ioopm_create_shopping_cart(database);
    }
    
    ioopm_list_t *shopping_cart = ioopm_hash_table_lookup(database->shopping_carts, database->username).pointer;
    ioopm_list_iterator_t *iter = ioopm_list_iterator(shopping_cart);

    while (ioopm_iterator_has_next(iter)) {
        cart_item_t *current_cart_item = ioopm_iterator_next(iter).pointer;
        merch_t *current_merch = current_cart_item->merch.pointer;
        
        if (0 == strcmp(current_merch->name.pointer, merch_name.pointer)) {
            int new_quantity = current_cart_item->quantity + quantity;
            merch_t *merch_something = find_merch(database, merch_name);
            int total_quantity = merch_something->total_quantity;

            if (total_quantity >= new_quantity) {
                current_cart_item->quantity = new_quantity;
                ioopm_iterator_destroy(iter);
                return true;
            } else {
                ioopm_iterator_destroy(iter);
                return false;
            }
        }
    }
    ioopm_iterator_destroy(iter);

    merch_t *merch = find_merch(database, merch_name);
    
    cart_item_t *cart_item  = calloc(1, sizeof(cart_item_t));
    cart_item->merch = ptr_elem(merch);
    cart_item->quantity = quantity;
    ioopm_linked_list_append(shopping_cart, ptr_elem(cart_item));
    return true;
}

bool ioopm_remove_from_cart(database_t *database, elem_t merch, int quantity) {
    assert(database);
    if (quantity <= 0) {
        return false;
    }

    ioopm_list_t *shopping_cart = ioopm_hash_table_lookup(database->shopping_carts, database->username).pointer;
    assert(shopping_cart);

    if (shopping_cart == NULL) {
        return false;
    }
    
    ioopm_list_iterator_t *iter = ioopm_list_iterator(shopping_cart);
    int index = 0;

    while (ioopm_iterator_has_next(iter)) {
        cart_item_t *current_cart_item = ioopm_iterator_next(iter).pointer;
        merch_t *m = current_cart_item->merch.pointer;
        if (0 == strcmp(m->name.pointer, merch.pointer) && current_cart_item->quantity > 0) {
            int new_quantity = current_cart_item->quantity - quantity;
            
            if (current_cart_item->quantity >= new_quantity && new_quantity > 0) {
                current_cart_item->quantity = new_quantity;
                ioopm_iterator_destroy(iter);
                return true;
            } else if (new_quantity == 0){
                ioopm_linked_list_remove(shopping_cart, index);
                free (current_cart_item);
                ioopm_iterator_destroy(iter);
                return true;
            } else {
                ioopm_iterator_destroy(iter);
                return false;
            }
        }
        index++;
    }
    ioopm_iterator_destroy(iter);
    return false;
}

int ioopm_calculate_cost(database_t *database) {
    assert(database);
    int total_cost = 0;
    if (ioopm_cart_exists(database)) {
    ioopm_list_t *shopping_cart = ioopm_hash_table_lookup(database->shopping_carts, database->username).pointer;
    ioopm_list_iterator_t *iter = ioopm_list_iterator(shopping_cart);
    int merch_cost = 0;
    while (ioopm_iterator_has_next(iter)) {
        cart_item_t *current_cart_item = ioopm_iterator_next(iter).pointer;
        merch_t *merch = current_cart_item->merch.pointer;
        merch_cost = current_cart_item->quantity * merch->price;
        total_cost = total_cost + merch_cost;
    }
    ioopm_iterator_destroy(iter);
    }
    return total_cost;
}
