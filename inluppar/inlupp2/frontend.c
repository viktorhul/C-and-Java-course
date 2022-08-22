#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

#include "frontend.h"
#include "business_logic.h"
#include "generic_utils.h"
#include "common.h"

struct token {
    int size;
    char **list;
};

#define TEXT_ALERT "\x1B[31m"
#define TEXT_RESET "\x1B[0m                    "

void render_body(token_t left, token_t right) {
    int row = 0;

    while (row < left.size || row < right.size) {
        char *left_content = (row < left.size) ? left.list[row] : "";
        char *right_content = (row < right.size) ? right.list[row] : "";

        printf(
            "| %-50s | %-30s |\n", 
            left_content,
            right_content
        );

        row++;
    }
}

void user_interact(
    void *res, 
    database_t *database, 
    token_t left_content,
    token_t right_content,
    int input, 
    valid_format_t option_format
) {
    char *error = "";

    while (true) {
        char *user = (database->username.pointer != NULL) ? database->username.pointer : "Not logged in";
        printf("---------------------------------------------------------------------------------------\n");
        printf("| Store Manager 2021                                 | USER: %-20s     |\n", user);
        printf("---------------------------------------------------------------------------------------\n");

        render_body(left_content, right_content);

        printf("---------------------------------------------------------------------------------------\n");

        if (0 != strcmp("", error)) {
            printf("| " TEXT_ALERT "%-63s" TEXT_RESET " |\n", error);
            error = "";
            printf("---------------------------------------------------------------------------------------\n");
        }

        if (false == read_input(input, option_format, res))
            error = "Invalid input";
        else
            break;
    }    
}

int user_get_row_number(database_t *database, char *type, int from, int to) {
    int row_number = 0;
    char q_modifier[50];
    snprintf(q_modifier, 50, "want to %s?", type);
    
    char *question_choose_number[2] = {
        "What merch (number) do you",
        q_modifier
    };
    
    char q_range[25];
    snprintf(q_range, 25, "Range: %d to %d", from, to);
    char *question_range[1] = { q_range };

    user_interact(
        &row_number,
        database,
        (token_t) { .size = 2, .list = question_choose_number },
        (token_t) { .size = 1, .list = question_range },
        INPUT_INT,
        OPTION_FORMAT_RANGE(from, to)
    );
    
    return row_number;
}

token_t fetch_list_interval(ioopm_list_t *list, int min, int max) {
    if (NULL == list)
        return (token_t) {0};

    ioopm_list_t *temp = ioopm_linked_list_create(ioopm_string_eq);

    int diff = max - min;
    int index = 0;
    while (index++ < diff) {
        elem_t item = ioopm_linked_list_get(list, min + index);

        if (NULL == item.pointer)
            continue;

        ioopm_linked_list_append(temp, item);
    }

    int actual_fetched_size = ioopm_linked_list_size(temp);

    if (0 == actual_fetched_size)
        return (token_t) {0};

    char **interval = calloc(actual_fetched_size, sizeof(char *));

    index = 0;
    int row_number = min + 1;

    ioopm_list_iterator_t *iter = ioopm_list_iterator(temp);
    while (ioopm_iterator_has_next(iter)) {
        char *item_buf = calloc(255, sizeof(char));
        char *item_str = ioopm_iterator_next(iter).pointer;
        snprintf(item_buf, 255, "%d) %s", row_number++, item_str);
        interval[index] = item_buf;
        index++;
    }
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(temp);

    return (token_t) { .size = actual_fetched_size, .list = interval };
}

void popup_message(database_t *database, token_t message) {
    char *confirm[1] = {
        "[Y] Okay"
    };

    char c = '\0';
    user_interact(
        &c,
        database,
        message,
        (token_t) { .size = 1, .list = confirm },
        INPUT_CHARS,
        OPTION_FORMAT_CHARS("Y")
    );

    switch (c) {
        case 'Y': return;
    }
}

void user_confirm_action(database_t *database) {
    char c = '\0';

    char *message[2] = {
        "Are you sure?",
        "You cannot undo this action"
    };

    char *options[2] = {
        "[Y] Yes, continue",
        "[N] No, cancel"
    };

    user_interact(
        &c,
        database,
        (token_t) { .size = 2, .list = message },
        (token_t) { .size = 2, .list = options },
        INPUT_CHARS,
        OPTION_FORMAT_CHARS("YN")
    );
}

bool user_edit_merch_form(database_t *database, elem_t merch_ptr) {
    merch_t *merch = merch_ptr.pointer;

    char new_name[255];
    char *name_info[2] = {
        "Type product name",
        merch->name.pointer
    };

    user_interact(
        &new_name,
        database,
        (token_t) { .size = 2, .list = name_info },
        (token_t) {0},
        INPUT_TEXT,
        OPTION_FORMAT_TEXT()
    );

    char new_desc[255];
    char *desc_info[2] = {
        "Type product description",
        merch->desc.pointer
    };

    user_interact(
        &new_desc,
        database,
        (token_t) { .size = 2, .list = desc_info },
        (token_t) {0},
        INPUT_TEXT,
        OPTION_FORMAT_TEXT()
    );

    int new_price = 0;
    char old_price[10];
    snprintf(old_price, 10, "%d", merch->price);
    char *price_info[2] = {
        "Type product price",
        old_price
    };

    user_interact(
        &new_price,
        database,
        (token_t) { .size = 2, .list = price_info },
        (token_t) {0},
        INPUT_INT,
        OPTION_FORMAT_RANGE(1, 999999)
    );

    return ioopm_edit_merch(database, merch->name, ptr_elem(new_name), ptr_elem(new_desc), new_price);
}

void user_edit_merch_details(database_t *database, elem_t merch_name) {
    elem_t merch = ioopm_hash_table_lookup(database->merch_items, merch_name);
    bool was_edited = user_edit_merch_form(database, merch);

    if (!was_edited) {
        char *question_confirm_edit[2] = {
            "The merch could not be edited.",
            "Make sure the new name does not already exist."
        };
        token_t message = { .size = 2, .list = question_confirm_edit };

        popup_message(database, message);
    }
}

void user_remove_merch_details(database_t *database, elem_t merch_name) {
    user_confirm_action(database);
    ioopm_remove_merch(database, merch_name);
}

void user_view_merch_details(database_t *database, elem_t merch_ptr) {
    ioopm_list_t *merch_list = ioopm_hash_table_keys(database->merch_items);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_list);
    merch_t *merch = NULL;

    while (ioopm_iterator_has_next(iter)) {
        elem_t m_name = ioopm_iterator_next(iter);

        if (0 == strcmp(merch_ptr.pointer, m_name.pointer)) {
            merch = ioopm_hash_table_lookup(database->merch_items, m_name).pointer;
            break;
        }
    }

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(merch_list);

    if (NULL == merch)
        return;

    char product_name[50];
    snprintf(product_name, 50, "Merch name: %s", (char *) merch->name.pointer);

    char product_desc[50];
    snprintf(product_desc, 50, "Description: %s", (char *) merch->desc.pointer);

    char product_price[50];
    snprintf(product_price, 50, "Price: %d", merch->price);

    char product_total_quantity[50];
    snprintf(product_total_quantity, 50, "Total quantity: %d", merch->total_quantity);

    char product_locations[255] = "";
    ioopm_list_t *locs = merch->locations;
    ioopm_list_iterator_t *locs_iter = ioopm_list_iterator(locs);
    int number_of_locations = ioopm_linked_list_size(locs);
    int n = 0;

    while (ioopm_iterator_has_next(locs_iter)) {
        shelf_t *shelf = ioopm_iterator_next(locs_iter).pointer;
        char mr[40];
        snprintf(
            mr, 
            40, 
            "%s (%d)%s", 
            (char *) shelf->shelf.pointer, 
            shelf->quantity,
            (++n < number_of_locations) ? ", " : ""
        );
        strcat(product_locations, mr);
    }

    ioopm_iterator_destroy(locs_iter);

    char *merch_info[5] = {
        product_name,
        product_desc,
        product_price,
        product_total_quantity,
        product_locations
    };

    popup_message(
        database,
        (token_t) { .size = 5, .list = merch_info }
    );
}

void user_initial_action(database_t *database, char *type, next_action next_action) {
    ioopm_list_t *merch_list = ioopm_hash_table_keys(database->merch_items);
    int merch_size = ioopm_hash_table_size(database->merch_items);
    int row_number = user_get_row_number(database, type, 1, merch_size);

    char c = '\0';
    elem_t merch_name = ioopm_linked_list_get(merch_list, row_number);

    ioopm_linked_list_destroy(merch_list);

    char confirm_text[30];
    snprintf(confirm_text, 30, "Do you want to %s:", type);

    char *question_confirm[2] = {
        confirm_text,
        (char *) merch_name.pointer
    };

    char *question_options[2] = {
        "[Y] Yes",
        "[N] No"
    };

    user_interact(
        &c,
        database,
        (token_t) { .size = 2, .list = question_confirm },
        (token_t) { .size = 2, .list = question_options },
        INPUT_CHARS,
        OPTION_FORMAT_CHARS("YN")
    );

    switch (c) {
        case 'Y': next_action(database, merch_name);
        case 'N': return;
    }
}

void user_add_to_cart_details(database_t *database, elem_t merch_name) {
    int units_in_stock = 0;
    int units_already_in_cart = 0;
    
    merch_t *merch_ptr = ioopm_hash_table_lookup(database->merch_items, merch_name).pointer;
    units_in_stock = merch_ptr->total_quantity;

    elem_t user_shopping_cart_ptr = ioopm_hash_table_lookup(database->shopping_carts, database->username);

    if (user_shopping_cart_ptr.pointer != NULL) {
        ioopm_list_iterator_t *shopping_cart_iter = ioopm_list_iterator(user_shopping_cart_ptr.pointer);
        while (ioopm_iterator_has_next(shopping_cart_iter)) {
            cart_item_t *c = ioopm_iterator_next(shopping_cart_iter).pointer;
            merch_t *m = c->merch.pointer;

            if (0 == strcmp(m->name.pointer, merch_ptr->name.pointer)) {
                units_already_in_cart = c->quantity;
                break;
            }
        }
        ioopm_iterator_destroy(shopping_cart_iter);
    }
    
    char units_in_stock_char[25];
    char units_already_in_cart_char[25];
    snprintf(units_in_stock_char, 25, "Units in stock: %d", units_in_stock);
    snprintf(units_already_in_cart_char, 25, "Units in cart: %d", units_already_in_cart);
    char *question[4] = {
        "How many do you want",
        "to add?",
        units_in_stock_char,
        units_already_in_cart_char
    };

    int units_available_to_add = units_in_stock - units_already_in_cart;

    char q_range[25];
    snprintf(q_range, 25, "Range: 1 to %d", units_available_to_add);
    char *question_range[1] = { q_range };

    int add_units_to_stock = 0;
    user_interact(
        &add_units_to_stock,
        database,
        (token_t) { .size = 4, .list = question },
        (token_t) { .size = 1, .list = question_range },
        INPUT_INT,
        OPTION_FORMAT_RANGE(1, units_available_to_add)
    );
    
    ioopm_add_to_cart(database, merch_name, add_units_to_stock);
}


void user_add_to_cart(database_t *database) {
    ioopm_list_t *merch_list = ioopm_hash_table_keys(database->merch_items);
    int merch_size = ioopm_hash_table_size(database->merch_items);
    int row_number = user_get_row_number(database, "add to cart", 1, merch_size);

    char c = '\0';
    char *merch_name = ioopm_linked_list_get(merch_list, row_number).pointer;
    int total_quantity = 0;
    int units_already_in_cart = 0;
    ioopm_linked_list_destroy(merch_list);
    merch_t *merch_ptr = ioopm_hash_table_lookup(database->merch_items, ptr_elem(merch_name)).pointer;

    total_quantity = merch_ptr->total_quantity;

    elem_t user_shopping_cart_ptr = ioopm_hash_table_lookup(database->shopping_carts, database->username);

    if (user_shopping_cart_ptr.pointer != NULL) {
        ioopm_list_iterator_t *shopping_cart_iter = ioopm_list_iterator(user_shopping_cart_ptr.pointer);
        while (ioopm_iterator_has_next(shopping_cart_iter)) {
            cart_item_t *c = ioopm_iterator_next(shopping_cart_iter).pointer;
            merch_t *m = c->merch.pointer;

            if (0 == strcmp(m->name.pointer, merch_ptr->name.pointer)) {
                units_already_in_cart = c->quantity;
                break;
            }
        }
        ioopm_iterator_destroy(shopping_cart_iter);
    }

    
    if (0 == total_quantity) {
    char *message_text[1] = {
            "The merch is out of stock"
        };
        token_t message = { .size = 1, .list = message_text };
        popup_message(database, message);
    } else if (total_quantity - units_already_in_cart <= 0) {
        char *message_text[2] = {
            "You have already put all available items",
            "in your shopping cart"
        };
        token_t message = { .size = 2, .list = message_text };
        popup_message(database, message);
    } else {
        char *question_confirm_edit[3] = {
            "Do you want to add",
            merch_name,
            "to your cart?"
        };

        char *question_confirm_options[2] = {
            "[Y] Yes",
            "[N] No, cancel"
        };

        user_interact(
            &c,
            database,
            (token_t) { .size = 3, .list = question_confirm_edit },
            (token_t) { .size = 2, .list = question_confirm_options },
            INPUT_CHARS,
            OPTION_FORMAT_CHARS("YN")
        );

        switch (c) {
            case 'Y': {
                user_add_to_cart_details(database, ptr_elem(merch_name));
                return;
            }
            case 'N': return;
        }
    }
}

void user_replenish_details(database_t *database, elem_t merch_name) {
    char shelf[4];
    char *question[2] = {
        "Type the shelf",
        "Format: [character] [number] [number]"
    };

    user_interact(
        &shelf,
        database,
        (token_t) { .size = 2, .list = question },
        (token_t) {0},
        INPUT_SHELF,
        OPTION_FORMAT_TEXT()
    );

    int quantity;
    char *question_int[1] = {
        "Type the quantity"
    };

    user_interact(
        &quantity,
        database,
        (token_t) { .size = 1, .list = question_int },
        (token_t) {0},
        INPUT_INT,
        OPTION_FORMAT_RANGE(1, 999999)
    );

    bool replenished = ioopm_replenish(database, merch_name, ptr_elem(shelf), quantity);

    if (!replenished) {
        char *message[2] = {
            "The merch could not be replenished.",
            "Make sure the shelf is not already occupied"
        };
        popup_message(
            database,
            (token_t) { .size = 2, .list = message }
        );
    }
}

void user_list_merch(database_t *database) {
    int merch_size = 0;
    float fraction = 0;
    int total_pages = 0;
    int page_size = 3;
    int current_page = 1;
    char page_header[20];

    while (true) {
        merch_size = ioopm_hash_table_size(database->merch_items);
        fraction = (float) merch_size / page_size;
        total_pages = ceil(fraction);

        if (current_page > total_pages)
            current_page = total_pages;
        
        if (merch_size <= 0)
            return;

        char c = '\0';
        ioopm_list_t *merch_list = ioopm_hash_table_keys(database->merch_items);

        int merch_from = (current_page - 1) * page_size;
        int merch_to = merch_from + page_size;

        token_t left_content = fetch_list_interval(merch_list, merch_from, merch_to);
        ioopm_linked_list_destroy(merch_list);
        
        snprintf(page_header, 20, "Page: %d of %d", current_page, total_pages);
        left_content.list = append_array(page_header, left_content.list, ++left_content.size);

        char *options_char = "";
        char *options[8] = {
            "[Z] Previous page",
            "[X] Next page",
            "[M] Main menu",
            "[E] Edit merch",
            "[R] Remove merch",
            "[V] View merch details",
            "[A] Add to cart",
            "[T] Replenish merch"
        };

        if (1 == total_pages) {
            options_char = "MERVAT";
            options[0] = "[-] Previous page";
            options[1] = "[-] Next page";
        } else if (1 == current_page) {
            options_char = "XMERVAT";
            options[0] = "[-] Previous page";
        } else if (current_page == total_pages) {
            options_char = "ZMERVAT";
            options[1] = "[-] Next page";
        } else {
            options_char = "ZXMERVAT";
        }

        token_t right_content = {
            .size = 8,
            .list = options
        };

        user_interact(
            &c,
            database,
            left_content,
            right_content,
            INPUT_CHARS,
            OPTION_FORMAT_CHARS(options_char)
        );

        for (int i = 1; i < left_content.size; i++)
            free(left_content.list[i]);
        free(left_content.list);

        switch (c) {
            case 'Z': {
                if (1 < current_page)
                    current_page--;
                break;
            }
            case 'X': {
                if (current_page < total_pages)
                    current_page++;
                break;
            }
            case 'M': {
                return;
            }
            case 'E': {
                user_initial_action(database, "edit", user_edit_merch_details);
                break;
            }
            case 'R': {
                user_initial_action(database, "remove", user_remove_merch_details);
                break;
            }
            case 'V': {
                user_initial_action(database, "view", user_view_merch_details);
                break;
            }
            case 'A': {
                user_add_to_cart(database);
                break;
            }
            case 'T': {
                user_initial_action(database, "replenish", user_replenish_details);
                break;
            }
        }
    }
}

void user_add_merch(database_t *database) {
    char name[255];
    char *name_info[1] = {
        "Type product name"
    };

    user_interact(
        &name,
        database,
        (token_t) { .size = 1, .list = name_info },
        (token_t) {0},
        INPUT_TEXT,
        OPTION_FORMAT_TEXT()
    );

    char desc[255];
    char *desc_info[1] = {
        "Type product description"
    };

    user_interact(
        &desc,
        database,
        (token_t) { .size = 1, .list = desc_info },
        (token_t) {0},
        INPUT_TEXT,
        OPTION_FORMAT_TEXT()
    );

    int price = 0;
    char *price_info[1] = {
        "Type product price"
    };

    user_interact(
        &price,
        database,
        (token_t) { .size = 1, .list = price_info },
        (token_t) {0},
        INPUT_INT,
        OPTION_FORMAT_RANGE(1, 999999)
    );

    ioopm_add_merch(database, ptr_elem(name), ptr_elem(desc), price);
}

void user_remove_from_cart_details(database_t *database, elem_t merch_name) {
    ioopm_list_t *user_cart = ioopm_hash_table_lookup(database->shopping_carts, database->username).pointer;
    int quantity = 0;
    ioopm_list_iterator_t *iter = ioopm_list_iterator(user_cart);
    while (ioopm_iterator_has_next(iter)) {
        cart_item_t *c = ioopm_iterator_next(iter).pointer;
        merch_t *m = c->merch.pointer;

        if (0 == strcmp(m->name.pointer, merch_name.pointer)) {
            quantity = c->quantity;
            break;
        }
    }
    ioopm_iterator_destroy(iter);
    ioopm_remove_from_cart(database, merch_name, quantity);
}

void user_remove_from_cart(database_t *database) {
    ioopm_list_t *user_cart = ioopm_hash_table_lookup(database->shopping_carts, database->username).pointer;
    int cart_size = ioopm_linked_list_size(user_cart);
    int row_number = user_get_row_number(database, "remove from cart", 1, cart_size);

    char c = '\0';
    cart_item_t *cart_item = ioopm_linked_list_get(user_cart, row_number).pointer;
    merch_t *m = cart_item->merch.pointer;
    elem_t merch_name = m->name;

    char *question_confirm[3] = {
        "Do you want to remove",
        (char *) merch_name.pointer,
        "from cart?"
    };

    char *question_options[2] = {
        "[Y] Yes",
        "[N] No"
    };

    user_interact(
        &c,
        database,
        (token_t) { .size = 3, .list = question_confirm },
        (token_t) { .size = 2, .list = question_options },
        INPUT_CHARS,
        OPTION_FORMAT_CHARS("YN")
    );

    switch (c) {
        case 'Y': user_remove_from_cart_details(database, merch_name);
        case 'N': return;
    }
}

void user_checkout(database_t *database) {
    int total_cost = ioopm_calculate_cost(database);
    char sum_char[30];
    snprintf(sum_char, 30, "You owe us %d SEK", total_cost);
    char *message[2] = {
        "Thank you for your purchase!",
        sum_char
    };

    popup_message(
        database,
        (token_t) { .size = 2, .list = message }
    );

    ioopm_check_out(database);
}

void user_view_cart_list(database_t *database, ioopm_list_t *user_cart) {
    int cart_items = 0;
    float fraction = 0;
    int total_pages = 0;
    int page_size = 3;
    int current_page = 1;
    char page_header[20];

    while (true) {
        cart_items = ioopm_linked_list_size(user_cart);
        if (cart_items <= 0)
            return;

        fraction = (float) cart_items / page_size;
        total_pages = ceil(fraction);

        char c = '\0';

        int cart_item_from = (current_page - 1) * page_size;
        int cart_item_to = cart_item_from + page_size;

        ioopm_list_t *user_cart_names = ioopm_linked_list_create(ioopm_string_eq);
        ioopm_list_iterator_t *iter = ioopm_list_iterator(user_cart);

        while (ioopm_iterator_has_next(iter)) {
            char *item = calloc(50, sizeof(char));
            cart_item_t *ci = ioopm_iterator_next(iter).pointer;
            merch_t *m = ci->merch.pointer;
            snprintf(item, 50, "%s (%d units)", (char *) m->name.pointer, ci->quantity);
            ioopm_linked_list_append(user_cart_names, ptr_elem(item));
        }

        ioopm_iterator_destroy(iter);

        token_t left_content = fetch_list_interval(user_cart_names, cart_item_from, cart_item_to);

        ioopm_list_iterator_t *left_iter = ioopm_list_iterator(user_cart_names);
        while (ioopm_iterator_has_next(left_iter)) {
            char *str = ioopm_iterator_next(left_iter).pointer;
            free(str);
        }
        ioopm_iterator_destroy(left_iter);
        ioopm_linked_list_destroy(user_cart_names);

        snprintf(page_header, 20, "Page: %d of %d", current_page, total_pages);        
        left_content.list = append_array(page_header, left_content.list, ++left_content.size);

        char *options_char = "";
        char *options[5] = {
            "[Z] Previous page",
            "[X] Next page",
            "[M] Main menu",
            "[R] Remove from cart",
            "[C] Checkout"
        };

        if (1 == total_pages) {
            options_char = "MRC";
            options[0] = "[-] Previous page";
            options[1] = "[-] Next page";
        } else if (1 == current_page) {
            options_char = "XMRC";
            options[0] = "[-] Previous page";
        } else if (current_page == total_pages) {
            options_char = "ZMRC";
            options[1] = "[-] Next page";
        } else {
            options_char = "ZXMRC";
        }

        token_t right_content = {
            .size = 5,
            .list = options
        };

        user_interact(
            &c,
            database,
            left_content,
            right_content,
            INPUT_CHARS,
            OPTION_FORMAT_CHARS(options_char)
        );

        for (int i = 1; i < left_content.size; i++) {
            free(left_content.list[i]);
        }
        free(left_content.list);

        switch (c) {
            case 'Z': {
                if (1 < current_page)
                    current_page--;
                break;
            }
            case 'X': {
                if (current_page < total_pages)
                    current_page++;
                break;
            }
            case 'M': {
                return;
            }
            case 'R': {
                user_remove_from_cart(database);
                break;
            }
            case 'C': {
                user_checkout(database);
                return;
            }
        }
    }
}

void user_view_cart(database_t *database) {
    ioopm_list_t *user_cart = ioopm_hash_table_lookup(database->shopping_carts, database->username).pointer;

    if (NULL == user_cart) {
        char *message[1] = {
            "There is no shopping cart"
        };
        popup_message(
            database, 
            (token_t) { .size = 1, .list = message }
        );
        return;
    } else if (0 == ioopm_linked_list_size(user_cart)) {
        char *message[1] = {
            "Shopping cart is empty"
        };
        popup_message(
            database, 
            (token_t) { .size = 1, .list = message }
        );
        return;
    } else {
        user_view_cart_list(database, user_cart);
        return;
    }
}

void ioopm_event_loop(database_t *database) {
    char user[255];

    char *message[3] = { 
        "Welcome!",
        "Type in your login",
        "credentials"
    };

    user_interact(
        user, 
        database, 
        (token_t) { .size = 3, .list = message }, 
        (token_t) {0}, 
        INPUT_TEXT, 
        OPTION_FORMAT_TEXT()
    );

    database->username = ptr_elem(user);

    while (true) {
        char *message2[2] = { 
            "Choose an option",
            "to the right"
        };

        char *options2[4] = {
            "[L] List all merch",
            "[A] Add merch",
            "[V] View shopping cart",
            "[Q] Quit program"
        };
        char c = '\0';

        user_interact(
            &c, 
            database, 
            (token_t) { .size = 2, .list = message2},
            (token_t) { .size = 4, .list = options2},
            INPUT_CHARS,
            OPTION_FORMAT_CHARS("LAVQ")
        );
        
        switch (c) {
            case 'L': {
                user_list_merch(database);
                break;
            }
            case 'A': { 
                user_add_merch(database);
                break;
            }
            case 'V': {
                user_view_cart(database);
                break;
            }
            case 'Q': return;
        }
    }
}