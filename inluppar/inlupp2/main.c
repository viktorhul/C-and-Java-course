#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "business_logic.h"
#include "generic_utils.h"
#include "frontend.h"
#include "common.h"

int main(void) {
    database_t *database = ioopm_create_database();

    ioopm_add_merch(database, ptr_elem("Butter"), ptr_elem("desc"), 10);
    ioopm_add_merch(database, ptr_elem("Grape"), ptr_elem("fruit"), 10);
    ioopm_add_merch(database, ptr_elem("Guava"), ptr_elem("fruit"), 50);

    ioopm_replenish(database, ptr_elem("Butter"), ptr_elem("A25"), 100);
    ioopm_replenish(database, ptr_elem("Grape"), ptr_elem("C35"), 100);
    ioopm_replenish(database, ptr_elem("Guava"), ptr_elem("E10"), 100);
    
    //ioopm_edit_merch(database, ptr_elem("Butter"), ptr_elem("Butter25"), ptr_elem("wow"), 50);

    ioopm_event_loop(database);
    
    ioopm_destroy_database(database);

    return 0;
} 