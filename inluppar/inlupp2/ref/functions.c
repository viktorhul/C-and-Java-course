#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "common.h"

void list_merch(void) {

}

ioopm_list_t *show_stock(merch_t *item) {
    return NULL;
}

void remove_merch(database_t *db, merch_t *item) {
    
    /*if (ioopm_hash_table_is_empty(db->merch_items))
    {
        puts("No merch yet");
    }
    else
    {
        //destroy all the copies in the storage location
        ioopm_list_t *item_at_locations = show_stock(item);
        ioopm_linked_list_destroy(item_at_locations);
        //remove item from the database
        ioopm_hash_table_remove(db->merch_items, item->name);
    }*/
}

/*

static bool string_eq(elem_t value1, elem_t value2)
{  
  if (!value1.pointer || !value2.pointer) return false;
  return (strcmp(value1.pointer, value2.pointer) == 0);
}


void edit_merch(database_t *db, merch_t *item, elem_t name, char *desc, int price) {
    //checks if item exists and name is available
    if (ioopm_hash_table_has_value(db->merch_items, item->name) && string_eq(item->name, name))
    {
        item->name = name;
        item->desc = desc;
        item->price = price;
    }
    else 
    {
        //TODO: lägga in errno? 
        puts("item does not exist or name's unavailable");
    }


}
*/

void replenish(database_t *db, elem_t name, elem_t shelf, int amount) {
    
}

void create_cart(void) {

}

void remove_cart(void) {

}

void add_to_cart(void) {

}

void remove_from_cart(void) {

}

void calculate_cost(void) {

}

void checkout_cart(void) {

}

// TODO: Frivillig
void undo_action(void) {

}

void quit_program(void) {

}

// TODO: Frivillig
void save_to_file(void) {

}