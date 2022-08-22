/*
 * USER INTERFACE
 * Everything that has to do with the program's user interface.
 * Functions like printing options and reading input of data specific
 * to the program in question belongs in this module.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#include "user_interface.h"
#include "business_logic.h"
#include "generic_utils.h"
#include "common.h"

#define TEXT_ALERT "\x1B[31m"
#define TEXT_RESET "\x1B[0m"

void ioopm_splash() {
    printf("-----------------------\n|  Store manager 2021 |\n-----------------------\n");
}

static void print_options(int merch_items) {
    char *choices[] = {
        "[A]dd merch",
        //"[R]emove merch",   // TODO: Possibly redundant
        //"[E]dit merch",     // TODO: Possibly redundant
        "[U]ndo action",
        "[L]ist all merch",
        "[Q]uit program",
        // "Add [T]o cart",
        "Remove [F]rom cart",
        "[V]iew cart",
        "[C]heckout"
    };
    int no_choices = sizeof(choices) / sizeof(char *);

    printf("What do you want to do?\n");

    for (int i = 0; i < no_choices; i++) {
        if (i == 2)
            printf("%s (%d items)\n", choices[i], merch_items);
        else
            printf("%s\n", choices[i]);
    }
}

static bool is_destructive_option(char option) {
    return (
        'R' == option ||
        'Q' == option
    );
}

// TODO: Förstå varför den beter sig underligt vid input > 2 (den borde bara kasta resten)
static char get_char_input() {
    int buf_siz = 2;
    char buf[buf_siz];
    bool input_is_one_char = ioopm_read_string(buf, buf_siz) == 1;

    return (input_is_one_char) ? buf[0] : '\0';
}

static bool confirm_option() {
    char option;

    while (true) {
        printf("Are you sure? You cannot undo this action. (y/n)\n");
        option = toupper(get_char_input());

        if ('Y' == option)
            return true;
        else if ('N' == option)
            return false;
    }
}

static char get_user_option(int merch_items) {
    char option;
    char *error_message = NULL;
    char *accepted_options = "AULQTFVC";

    while (true) {
        ioopm_splash();

        if (error_message)
            printf(TEXT_ALERT "%s\n" TEXT_RESET, error_message);
        error_message = NULL;

        print_options(merch_items);

        option = toupper(get_char_input());
        bool option_exists = strchr(accepted_options, option) != NULL;

        if ('\0' == option || !option_exists) {
            error_message = "Not a valid choice!";
            continue;
        }

        if (is_destructive_option(option) && !confirm_option()) {
            error_message = NULL;
            continue;
        } else
            break;
    }

    return option;
}

// TODO: Remove? Possibly redundant
bool ioopm_char_to_int(char *str, int *result) {
    while (*str++ != '\0')
        if (!isdigit(str))
            return false;

    *result = atoi(str);
    return true;
}

static bool get_valid_option(char *option, char *accepted_options) {
    char input = toupper(get_char_input());

    bool valid_option = (strchr(accepted_options, input) != NULL && input != '\0' && input != '\n');

    if (valid_option)
        *option = input;

    return valid_option;
}

int ioopm_ask_for_row(char *type) {
    printf("What merch do you want to %s? ", type);
    int size = 50;
    char answer[size];
    ioopm_read_string(answer, size);
    return atoi(answer);
}

void ioopm_frontend_add_merch(database_t *database) {
    char *name = calloc(255, sizeof(char));
    bool invalid_merch_name = false;
    int read_size = 0;

    while (0 >= read_size || invalid_merch_name) {
        printf("Merch name: ");
        read_size = ioopm_read_string(name, 255);
        invalid_merch_name = ioopm_merch_exists(database, ptr_elem(name));
    }

    char *desc = calloc(255, sizeof(char));

    printf("Merch description: ");
    ioopm_read_string(desc, 255);

    int price_size = 10;
    char price[price_size];

    do {
        printf("Merch price: ");
    } while (0 >= ioopm_read_string(price, price_size) && 0 > atoi(price));

    ioopm_add_merch(database, ptr_elem(name), ptr_elem(desc), atoi(price));
}

void ioopm_frontend_edit_merch(database_t *database, elem_t merch) {
    merch_t *old_merch = ioopm_hash_table_lookup(database->merch_items, merch).pointer;

    int name_size = 255;
    char *name = calloc(name_size, sizeof(char));
    bool invalid_merch_name = false;
    int read_size = 0;

    while (0 >= read_size || invalid_merch_name) {
        printf("Merch name (%s): ", (char *) old_merch->name.pointer);
        read_size = ioopm_read_string(name, name_size);
        //invalid_merch_name = ioopm_merch_exists(database, ptr_elem(name));
    }

    int desc_size = 255;
    char *desc = calloc(desc_size, sizeof(char));

    printf("Merch description (%s): ", (char *) old_merch->desc.pointer);
    ioopm_read_string(desc, desc_size);

    int price_size = 10;
    char price[price_size];

    do {
        printf("Merch price (%d): ", old_merch->price);
    } while (0 >= ioopm_read_string(price, price_size) && 0 > atoi(price));

    bool merch_edited = ioopm_edit_merch(database, merch, ptr_elem(name), ptr_elem(desc), atoi(price));

    if (!merch_edited)
        printf("Error: merch was not edited\n");
    else
        printf("Merch edited successfully\n");
}

void ioopm_frontend_view_stock(database_t *database, elem_t merch) {
    merch_t *m = ioopm_hash_table_lookup(database->merch_items, merch).pointer;
    printf(
        "Merch name: %s\nDescription: %s\nPrice: %d\nQuantity: %d\n",
        (char *) m->name.pointer,
        (char *) m->desc.pointer,
        m->price,
        m->total_quantity
    );

    ioopm_list_t *locations = ioopm_show_stock(m);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(locations);

    while (ioopm_iterator_has_next(iter)) {
        shelf_t *shelf = ioopm_iterator_next(iter).pointer;
        printf("%s: %d\n", (char *) shelf->shelf.pointer, shelf->quantity);
    }

    ioopm_iterator_destroy(iter);
}

void ioopm_list_merch_new(database_t *database) {
    int page_size = 2;
    int merch_size = ioopm_hash_table_size(database->merch_items);
    float fraction = (float) merch_size / page_size;
    int total_pages = ceil(fraction);
    int current_page = 1;

    while (true) {
        merch_size = ioopm_hash_table_size(database->merch_items);
        fraction = (float) merch_size / page_size;
        total_pages = ceil(fraction);

        if (merch_size <= 0)
            return;

        char options_print[200];
        snprintf(
            options_print,
            200,
            "Options: %s%s%s",
            (current_page != 1) ? "previous page (A), " : "",
            (current_page != total_pages) ? "next page (S), " : "",
            "main menu (M), edit merch (E), remove merch (R), view merch details (V), add to cart (T)"
        );

        ioopm_splash();
        printf("%s\n", options_print);
        printf("Page: %d of %d\n", current_page, total_pages);

        int row_number = 1;
        ioopm_list_t *merch_list = ioopm_hash_table_keys(database->merch_items);
        ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_list);

        int iterations = (current_page - 1) * page_size;
        while (iterations-- > 0) {
            ioopm_iterator_next(iter);
            row_number++;
        }

        while (ioopm_iterator_has_next(iter) && row_number <= (page_size * current_page)) {
            char *merch_name = ioopm_iterator_next(iter).pointer;
            printf("%d) %s\n", row_number, merch_name);
            row_number++;
        }
        ioopm_iterator_destroy(iter);

        char option = '\0';
        bool correct_option = false;
        char *accepted_options = 
            (total_pages == 1) ? "MERVT" :
            (current_page == 1) ? "SMERVT" :
            (current_page == total_pages) ? "AMERVT" :
            "ASMERVT";

        while ((correct_option = get_valid_option(&option, accepted_options) == false)) {
            printf(TEXT_ALERT "Invalid input " TEXT_RESET "%s\n", options_print);
        }

        elem_t merch_to_modify = ptr_elem(NULL);
        int row_to_modify = 0;
        int total_quantity = 0;

        switch (option) {
            case 'A':
                // Previous page
                if (current_page > 1)
                    current_page--;

                continue;

            case 'S':
                // Next page
                if (current_page < total_pages)
                    current_page++;

                continue;

            case 'M':
                // Return to main menu
                return;

            case 'E':
                row_to_modify = ioopm_ask_for_row("edit");
                while (!(row_to_modify <= merch_size && row_to_modify > 0)) {
                    printf(TEXT_ALERT "Invalid row number " TEXT_RESET);
                    row_to_modify = ioopm_ask_for_row("edit");
                }

                merch_to_modify = ioopm_linked_list_get(merch_list, row_to_modify);

                printf("Do you want to edit %s? (y/n)\n", (char *) merch_to_modify.pointer);

                option = '\0';
                while ((correct_option = get_valid_option(&option, "YN") == false)) {
                    printf(TEXT_ALERT "Invalid input " TEXT_RESET " (y/n)");
                }

                switch (option) {
                    case 'Y':
                        ioopm_frontend_edit_merch(database, merch_to_modify);
                        break;
                    
                    case 'N':
                        break;
                }

                break;

            case 'R':
                row_to_modify = ioopm_ask_for_row("remove");
                while (!(row_to_modify <= merch_size && row_to_modify > 0)) {
                    printf(TEXT_ALERT "Invalid row number " TEXT_RESET);
                    row_to_modify = ioopm_ask_for_row("remove");
                }

                merch_to_modify = ioopm_linked_list_get(merch_list, row_to_modify);

                printf("Do you want to remove %s? (y/n)\n", (char *) merch_to_modify.pointer);

                option = '\0';
                while ((correct_option = get_valid_option(&option, "YN") == false)) {
                    printf(TEXT_ALERT "Invalid input " TEXT_RESET " (y/n)");
                }

                switch (option) {
                    case 'Y':
                        confirm_option();

                        ioopm_remove_merch(database, merch_to_modify);

                        printf("%s was removed\n", (char *) merch_to_modify.pointer);
                        
                        break;
                    
                    case 'N':
                        break;
                }

                break;

            case 'V':
                row_to_modify = ioopm_ask_for_row("view");
                while (!(row_to_modify <= merch_size && row_to_modify > 0)) {
                    printf(TEXT_ALERT "Invalid row number " TEXT_RESET);
                    row_to_modify = ioopm_ask_for_row("view");
                }

                merch_to_modify = ioopm_linked_list_get(merch_list, row_to_modify);

                ioopm_frontend_view_stock(database, merch_to_modify);
                break;

            case 'T':
                // Add to cart
                row_to_modify = ioopm_ask_for_row("add to cart");
                while (!(row_to_modify <= merch_size && row_to_modify > 0)) {
                    printf(TEXT_ALERT "Invalid row number " TEXT_RESET);
                    row_to_modify = ioopm_ask_for_row("add to cart");
                }

                merch_to_modify = ioopm_linked_list_get(merch_list, row_to_modify);

                merch_t *m = merch_to_modify.pointer;
                total_quantity = m->total_quantity;

                if (total_quantity <= 0) {
                    printf("This merch is out of stock\n");
                    break;
                }
                else {
                row_to_modify = ioopm_ask_for_row("How many do you want to add?");
                while (!(row_to_modify <= total_quantity && row_to_modify > 0)) {
                    printf(TEXT_ALERT "Invalid quantity" TEXT_RESET);
                    row_to_modify = ioopm_ask_for_row("How many do you want to add?");
                } 
                ioopm_add_to_cart(database, merch_to_modify, row_to_modify);
                }

                break;
        }
    }
}



void ioopm_get_username(database_t *database) {
    int buf_siz = 255;
    char *input = calloc(buf_siz, sizeof(char *));

    do {
        printf("Login: ");
    } while (0 == ioopm_read_string(input, buf_siz));

    database->username = ptr_elem(input);
}

void ioopm_view_cart(database_t *database) {
    elem_t user = database->username;
    printf("view: %s\n", (char *) user.pointer);

    ioopm_list_t *shopping_cart = ioopm_hash_table_lookup(database->shopping_carts, user).pointer;
    ioopm_list_iterator_t *iter = ioopm_list_iterator(shopping_cart);
    if (ioopm_cart_is_empty(shopping_cart)) {
        printf("No items\n");
    }
    while (ioopm_iterator_has_next(iter)) {
        cart_item_t *current_cart_item = ioopm_iterator_next(iter).pointer;
        printf("%s, %d units\n", (char *) current_cart_item->merch.pointer, current_cart_item->quantity);
    }

    ioopm_iterator_destroy(iter);
}


void ioopm_frontend_remove_from_cart(database_t *database) {
    int current_page = 1;
    int page_size = 2;
    int merch_size = ioopm_hash_table_size(database->merch_items);
    elem_t merch_to_modify = ptr_elem(NULL);
    int row_to_modify = 0;

    int row_number = 1;
    ioopm_list_t *shopping_list = ioopm_hash_table_lookup(database->shopping_carts, database->username).pointer;
    ioopm_list_iterator_t *iter = ioopm_list_iterator(shopping_list);

    int iterations = (current_page - 1) * page_size;
    while (iterations-- > 0) {
        ioopm_iterator_next(iter);
        row_number++;
    }

    while (ioopm_iterator_has_next(iter) && row_number <= (page_size * current_page)) {
        merch_t *merch = ioopm_iterator_next(iter).pointer;
        char *merch_name = (char*)(merch->name).pointer;
        printf("%d) %s\n", row_number, merch_name);
        row_number++;
    }
    ioopm_iterator_destroy(iter);

    row_to_modify = ioopm_ask_for_row("remove?");
    while (!(row_to_modify <= merch_size && row_to_modify > 0)) {
        printf(TEXT_ALERT "Invalid row number " TEXT_RESET);
        row_to_modify = ioopm_ask_for_row("remove?");
    }

    merch_to_modify = ioopm_linked_list_get(shopping_list, row_to_modify);
    cart_item_t *current_cart_item =  merch_to_modify.pointer;

    row_to_modify = ioopm_ask_question_int("How many do you want to remove? ");
    while (!(row_to_modify <= current_cart_item->quantity && row_to_modify > 0)) {
    printf(TEXT_ALERT "Invalid quantity\n" TEXT_RESET);
    row_to_modify = ioopm_ask_question_int("How many do you want to remove? ");
    }
    ioopm_remove_from_cart(database, current_cart_item->merch, row_to_modify);
}


void ioopm_event_loop(database_t *database) {
    while (true) {
        int merch_items = ioopm_hash_table_size(database->merch_items);
        char option = get_user_option(merch_items);
        
        switch (option) {
            case 'A':
                // Add merch
                ioopm_frontend_add_merch(database);
                break;

            case 'U':
                // Undo action
                // OPTIONAL
                break;

            case 'L':
                // List all merch
                ioopm_list_merch_new(database);
                break;

            case 'Q':
                // Quit program
                // OPTIONAL: Save data to file
                return;
                break;

            case 'T':
                // Add to cart
                ioopm_list_merch_new(database);
                break;

            case 'F':
                ioopm_frontend_remove_from_cart(database);
                break;

            case 'V':
                ioopm_view_cart(database);
                break;

            case 'C':
                ioopm_check_out(database);
                break;
        }
    }

    return;
}