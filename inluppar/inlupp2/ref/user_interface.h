
#include "common.h"

/// @brief 
/// @param database store database
void ioopm_event_loop(database_t *database);

void ioopm_splash();

void ioopm_list_merch(database_t *database, bool show_row_number, bool pick_row_number);
void ioopm_list_merch_new(database_t *database);

void ioopm_get_username(database_t *database);
