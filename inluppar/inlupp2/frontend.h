#include "common.h"

/**
 * @file frontend.c
 * @author Viktor Hultsten & Patricia Müller
 * @date 4 Nov 2021
 * @brief A simple webstore frontend containing all
 * user intraction functions
 * 
 */

typedef void(*next_action)(database_t *database, elem_t merch_name);

/// Contains data to be printed to the screen, size is number of rows and
/// list is an array of strings (rows)
typedef struct token token_t;

/// @brief prints data to the left and right container of the output section in the
/// TUI template
/// @param left data to be printed in the left container
/// @param right data to be printed in the right container
void render_body(token_t left, token_t right);

/// @brief renders the TUI template
/// @param res pointer to the input from user
/// @param database database with all data
/// @param left_content text for the left output section
/// @param right_content text for the right output section
/// @param input accepted input type (int, char, etc)
/// @param option_format accepted data within the limits defined here
void user_interact(void *res, database_t *database, token_t left_content, 
    token_t right_content, int input, valid_format_t option_format);

/// @brief asks the user for a row number in a range defined by the caller
/// @param database the database
/// @param type text to combine with the question in the output section
/// @param from lower limit integer
/// @param to upper limit integer
/// @returns the integer within the range given by the user 
int user_get_row_number(database_t *database, char *type, int from, int to);

/// @brief fetches a range of elements from a linked list
/// @param list to fetch data from
/// @param min first element in index
/// @param max last element in index
/// @returns container ready to be sent to the output text section
token_t fetch_list_interval(ioopm_list_t *list, int min, int max);

/// @brief sends a simple message to the user 
/// @param database the database
/// @param message a container with the message to be printed
void popup_message(database_t *database, token_t message);

/// @brief confirms with the user that the action is wanted
/// @param database the database
void user_confirm_action(database_t *database);

/// @brief guides the user through a merch edit form and sends the data to backend
/// @param database the database
/// @param merch_ptr pointer to the merch_t
/// @returns true if value was edited in backend
bool user_edit_merch_form(database_t *database, elem_t merch_ptr);

/// @brief calls the edit form and returns an error to the user if not
/// @param database the database
/// @param merch_name pointer to the merch name
void user_edit_merch_details(database_t *database, elem_t merch_name);

/// @brief confirms that the user wants to continue and then calls backend function
/// @param database the database
/// @param merch_name pointer to the merch name
void user_remove_merch_details(database_t *database, elem_t merch_name);

/// @brief fetches and prints the merch details
/// @param database the database
/// @param merch_ptr pointer to the merch_t
void user_view_merch_details(database_t *database, elem_t merch_ptr);

/// @brief confirmation template that confirms the action and calls the 
/// appropriate function
/// @param database the database
/// @param type text to combine with the question to the user
/// @param next_action the function to be called if the user continues
void user_initial_action(database_t *database, char *type, next_action next_action);

/// @brief prints merch stock data
/// @param database the database
/// @param merch_name pointer to merch name
void user_add_to_cart_details(database_t *database, elem_t merch_name);

/// @brief lets the user add an appropriate amount of the merch, given that 
/// the merch is in stock
/// @param database the database
void user_add_to_cart(database_t *database);

/// @brief guides the user through a form to replenish a merch
/// @param database the database
/// @param merch_name pointer to the merch name
void user_replenish_details(database_t *database, elem_t merch_name);

/// @brief prints and guides the user through the listing function
/// @param database the database
void user_list_merch(database_t *database);

/// @brief guides the user through an add merch form and then sends it to backend
/// @param database the database
void user_add_merch(database_t *database);

/// @brief finds the merch in shopping cart and calls backend to remove it
/// @param database the database
/// @param merch_name pointer to merch name
void user_remove_from_cart_details(database_t *database, elem_t merch_name);

/// @brief calculates cost, checks out the user and prints a sum
/// @param database the database
void user_checkout(database_t *database);

/// @brief fetches and prints all shopping cart items and their quantity
/// @param database the database
/// @param user_cart the user's shopping cart
void user_view_cart_list(database_t *database, ioopm_list_t *user_cart);

/// @brief checks if shopping cart is empty, or calls the listing function
/// @param database the database
void user_view_cart(database_t *database);

/// @brief main menu loop that breaks only if the user want to quit the program
/// @param database the database
void ioopm_event_loop(database_t *database);