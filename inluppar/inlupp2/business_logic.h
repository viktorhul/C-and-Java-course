#include "inlupp1_ref/hash_table.h"
#include "inlupp1_ref/linked_list.h"
#include "inlupp1_ref/iterator.h"
#include "inlupp1_ref/common.h"
#include "common.h"

/**
 * @file business_logic.c
 * @author Viktor Hultsten & Patricia Müller
 * @date 4 Nov 2021
 * @brief A simple webstore backend containing all
 * database related functions
 * 
 */

/// @brief creates a database
/// @return a pointer to the created database
database_t *ioopm_create_database();

/// @brief destroys the database and everything it contains
/// @param database store database
void ioopm_destroy_database(database_t *database);

/// @brief adds merch to list
/// @param database store database
/// @param name name of the merch to be added
/// @param desc merch's description
/// @param price price
/// @return true if merch was added
bool ioopm_add_merch(database_t *database, elem_t name, elem_t desc, int price);

/// @brief removes merch from database
/// @param db database
/// @param name name of the merch to be removed 
/// @return true if merch was removed
bool ioopm_remove_merch(database_t *db, elem_t name);

/// @brief check if merch is in database
/// @param db database
/// @param name name of the merch  
/// @return true if merch exist in the database
bool ioopm_merch_exists(database_t *database, elem_t name);

/// @brief edit an existing merch
/// @param db database
/// @param name name of the merch to be edited
/// @param new_name name to be changed to   
/// @param new_desc the merch's new description  
/// @param new_price the merch's new price  
/// @return true if merch was edited successfully 
bool ioopm_edit_merch(database_t *database, elem_t name, elem_t new_name, elem_t new_desc, int new_price);

/// @brief restock existing merch
/// @param db database
/// @param name name of merch
/// @param shelf_name name of shelf to store merch
/// @param units quantity of the particular merch 
/// @return sorted merch in a new linked list
bool ioopm_replenish(database_t *database, elem_t name, elem_t shelf_name, int units);

/// @brief checks if there's any merch on the shelf
/// @param db database
/// @param shelf the shelf to be checked
/// @return true if there's merch on the shelf
bool ioopm_shelf_occupied(database_t *database, elem_t shelf);

/// @brief checks if shelf exists in the database
/// @param db database
/// @param shelf the shelf to be checked
/// @return true if shelf exists in the database
bool ioopm_shelf_exists(database_t *database, elem_t shelf);

/// @brief shows stock of a merch
/// @param name name of the merch
/// @return a linked list containing information of the given merch
ioopm_list_t *ioopm_show_stock(merch_t *name);

/// @brief checks if the shopping cart is empty
/// @param shooping_cart an existing shopping cart
/// @return true if the cart is empty
bool ioopm_cart_is_empty(ioopm_list_t *shopping_cart);

/// @brief checks if there is a cart connected to the current user
/// @param database database
/// @return true if the current user has a shopping cart
bool ioopm_cart_exists(database_t *database);

/// @brief creates a new shopping cart
/// @param database database
void ioopm_create_shopping_cart(database_t *database);

/// @brief adds a new merch to the shopping cart
/// @param database database
/// @param merch name of the merch
/// @param quantity the merch's quantities to be added
/// @return true if merch was added 
bool ioopm_add_to_cart(database_t *database, elem_t merch, int quantity);

/// @brief removes a merch from a shopping cart
/// @param database database
/// @param merch name of the merch
/// @param quantity the merch's quantities to be removed from cart
/// @return true if merch was removed
bool ioopm_remove_from_cart(database_t *database, elem_t merch, int quantity);

/// @brief calculates total cost of all items in a shopping cart
/// @param database database
/// @return total cost of all items
int ioopm_calculate_cost(database_t *database);

/// @brief checks out user's shopping cart and updates stock 
/// @param database database
void ioopm_check_out(database_t *database);

merch_t *find_merch(database_t *database, elem_t merch_name);
