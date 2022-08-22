#include <CUnit/Basic.h>
#include <stdlib.h>

#include "business_logic.h"
#include "generic_utils.h"
#include "common.h"

void test_create_destroy_database(void) {
    database_t *database = ioopm_create_database();
    ioopm_destroy_database(database);
}

void test_add_remove_merch(void) {
    database_t *database = ioopm_create_database();
    elem_t desc = ptr_elem("desc");
    int price = 100;

    // Add merch with expected names
    CU_ASSERT_TRUE( ioopm_add_merch(database, ptr_elem("Milk"), desc, price) );
    CU_ASSERT_TRUE( ioopm_add_merch(database, ptr_elem("Computers"), desc, price) );
    CU_ASSERT_TRUE( ioopm_add_merch(database, ptr_elem("Drugs"), desc, price) );

    //CU_ASSERT_TRUE( merch_exists(database, ptr_elem(dup)) );

    // Add already existing merch
    CU_ASSERT_FALSE( ioopm_add_merch(database, ptr_elem("Milk"), desc, price) );

    // Add a pretty odd name
    CU_ASSERT_TRUE( ioopm_add_merch(database, ptr_elem("A couple of spaces and 000000000012 numbers and these things: ##¤&(). This should work"), desc, price) );

    // Add merch without a name
    CU_ASSERT_FALSE( ioopm_add_merch(database, ptr_elem(""), desc, price) );

    // REMOVING MERCH

    // Remove some merch
    CU_ASSERT_TRUE( ioopm_remove_merch(database, ptr_elem("Milk")) );
    CU_ASSERT_TRUE( ioopm_remove_merch(database, ptr_elem("Computers")) );

    CU_ASSERT_FALSE( ioopm_merch_exists(database, ptr_elem("Milk")) );
    CU_ASSERT_FALSE( ioopm_merch_exists(database, ptr_elem("Computers")) );
    CU_ASSERT_TRUE( ioopm_merch_exists(database, ptr_elem("Drugs")) );

    // Remove non-existing merch
    CU_ASSERT_FALSE( ioopm_remove_merch(database, ptr_elem("Non-existing product")) );
    //destroy_merch_items(database);
    ioopm_destroy_database(database);
}

void test_edit_merch(void) {
    database_t *database = ioopm_create_database();

    elem_t desc = ptr_elem("desc");
    int price = 100;

    elem_t new_desc = ptr_elem("wow this is a new desc");
    int new_price = 252;

    // Add one merch and immediately edit it
    CU_ASSERT_TRUE( ioopm_add_merch(database, ptr_elem("Milk"), desc, price) );
    CU_ASSERT_TRUE( ioopm_edit_merch(database, ptr_elem("Milk"), ptr_elem("Orange juice"), new_desc, new_price) );
    
    // Replenish the new one and then edit it again
    ioopm_replenish(database, ptr_elem("Orange juice"), ptr_elem("A25"), 10);
    CU_ASSERT_TRUE( ioopm_edit_merch(database, ptr_elem("Orange juice"), ptr_elem("Funky goo"), desc, price) );
    
    //// Edit non-existing merch
    //CU_ASSERT_FALSE( ioopm_edit_merch(database, ptr_elem("Computers"), ptr_elem("Laptops"), new_desc, new_price) );

    //// Add some more merch
    CU_ASSERT_TRUE( ioopm_add_merch(database, ptr_elem("Computers"), desc, price) );
    CU_ASSERT_TRUE( ioopm_add_merch(database, ptr_elem("Apple juice"), desc, price) );
    
    //// Edit a product name to already existing name
    CU_ASSERT_TRUE( ioopm_add_merch(database, ptr_elem("Bread"), desc, price) );
    
    CU_ASSERT_FALSE( ioopm_edit_merch(database, ptr_elem("Apple juice"), ptr_elem("Bread"), desc, price) );
    
    //// Price change on the same merch
    CU_ASSERT_TRUE( ioopm_edit_merch(database, ptr_elem("Bread"), ptr_elem("Bread"), desc, new_price) );
    // Description change on the same merch
    CU_ASSERT_TRUE( ioopm_edit_merch(database, ptr_elem("Bread"), ptr_elem("Bread"), ptr_elem("Brand new desc"), price) );
    //destroy_merch_items(database);
    ioopm_destroy_database(database);
}

void test_add_stock(void) {
    database_t *database = ioopm_create_database();

    ioopm_add_merch(database, ptr_elem("Milk"), ptr_elem("desc"), 10);
    ioopm_add_merch(database, ptr_elem("Butter"), ptr_elem("desc"), 10);

    // Replenish existing merch
    CU_ASSERT_TRUE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("A25"), 5) );

    // Replenish exisiting merch on occupied shelf 
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Butter"), ptr_elem("A25"), 5) );

    // Replenish non-existing merch
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Rotten milk"), ptr_elem("A25"), 5) );

    // Replenish existing merch with weird amount of units
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("A25"), -5) );
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("A25"), 0) );

    // Replenish with empty fields
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem(""), ptr_elem("A25"), 5) );
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem(""), 5) );

    // Replenish on shelf with valid/invalid formats
    CU_ASSERT_TRUE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("B30"), 5) );
    CU_ASSERT_TRUE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("O56"), 5) );
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem(""), 5) );
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("BB30"), 5) );
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("B3"), 5) );
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("B"), 5) );
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("5"), 5) );
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("#12"), 5) );
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("-12"), 5) );
    CU_ASSERT_FALSE( ioopm_replenish(database, ptr_elem("Milk"), ptr_elem(" 12"), 5) );
    
    //destroy_merch_items(database);
    ioopm_destroy_database(database);
}

void test_add_to_cart(void) {
    database_t *database = ioopm_create_database();
    database->username = ptr_elem("test");

    ioopm_add_merch(database, ptr_elem("Milk"), ptr_elem("desc"), 10);

    ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("A25"), 5);
    ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("B30"), 6);
    ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("C35"), 7);

    CU_ASSERT_TRUE( ioopm_add_to_cart(database, ptr_elem("Milk"), 5) );
    CU_ASSERT_TRUE( ioopm_add_to_cart(database, ptr_elem("Milk"), 5) );
    CU_ASSERT_TRUE( ioopm_add_to_cart(database, ptr_elem("Milk"), 8) );
    CU_ASSERT_FALSE( ioopm_add_to_cart(database, ptr_elem("Milk"), 1) );
    CU_ASSERT_FALSE( ioopm_add_to_cart(database, ptr_elem("Milk"), 5) );
    CU_ASSERT_FALSE( ioopm_add_to_cart(database, ptr_elem("Milk"), 0) );
    CU_ASSERT_FALSE( ioopm_add_to_cart(database, ptr_elem("Milk"), -5) );

    //destroy_merch_items(database);
    ioopm_destroy_database(database);
}

void test_remove_from_cart(void) {
    database_t *database = ioopm_create_database();
    database->username = ptr_elem("test");

    ioopm_add_merch(database, ptr_elem("Milk"), ptr_elem("desc"), 10);
    ioopm_add_merch(database, ptr_elem("Butter"), ptr_elem("desc"), 10);
    ioopm_add_to_cart(database, ptr_elem("Milk"), 6);
    CU_ASSERT_TRUE( ioopm_remove_from_cart(database, ptr_elem("Milk"), 4) ); 
    CU_ASSERT_FALSE( ioopm_remove_from_cart(database, ptr_elem("Milk"), 100) ); // remove more than stock's & cart's quantity
    CU_ASSERT_FALSE( ioopm_remove_from_cart(database, ptr_elem("Apple"), 1) ); // merch doesn't exists in database
    CU_ASSERT_FALSE( ioopm_remove_from_cart(database, ptr_elem("Butter"), 1) ); // merch exists in database but not in cart
    CU_ASSERT_FALSE( ioopm_remove_from_cart(database, ptr_elem("Butter"), -1) ); // negative number
    
    //destroy_merch_items(database);
    ioopm_destroy_database(database);
}

void test_calculate_cost(void) {
    database_t *database = ioopm_create_database();
    database->username = ptr_elem("test");
    ioopm_add_merch(database, ptr_elem("Milk"), ptr_elem("desc"), 5);
    ioopm_add_merch(database, ptr_elem("Butter"), ptr_elem("desc"), 10);
    ioopm_add_merch(database, ptr_elem("Avocado"), ptr_elem("desc"), 50);
    CU_ASSERT_EQUAL( ioopm_calculate_cost(database), 0); // TODO: bestämma returvärdet när vi inte har en cart
    
    ioopm_add_to_cart(database, ptr_elem("Milk"), 5);
    ioopm_add_to_cart(database, ptr_elem("Avocado"), 5);
    CU_ASSERT_EQUAL( ioopm_calculate_cost(database), 275);

    ioopm_remove_from_cart(database, ptr_elem("Milk"), 2);
    ioopm_remove_from_cart(database, ptr_elem("Avocado"), 3);  
    CU_ASSERT_EQUAL( ioopm_calculate_cost(database), 115);

    //destroy_merch_items(database);
    ioopm_destroy_database(database);
}

void test_check_out(void) {
    database_t *database = ioopm_create_database();
    database->username = ptr_elem("test");

    ioopm_add_merch(database, ptr_elem("Milk"), ptr_elem("desc"), 10);
    ioopm_add_merch(database, ptr_elem("Butter"), ptr_elem("desc"), 10);
    ioopm_replenish(database, ptr_elem("Milk"), ptr_elem("A25"), 11);
    ioopm_replenish(database, ptr_elem("Butter"), ptr_elem("B30"), 100);
    ioopm_add_to_cart(database, ptr_elem("Milk"), 5);
    ioopm_add_to_cart(database, ptr_elem("Butter"), 50);

    ioopm_check_out(database);

    merch_t *merch1 = find_merch(database, ptr_elem("Milk"));
    merch_t *merch2 = find_merch(database, ptr_elem("Butter"));

    CU_ASSERT_EQUAL(merch1->total_quantity, 6);
    CU_ASSERT_EQUAL(merch2->total_quantity, 50);
    CU_ASSERT_FALSE(ioopm_cart_exists(database));
    //destroy_merch_items(database);
    ioopm_destroy_database(database);
}

int main(void) {
    CU_pSuite test_suite1 = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // add a suite to the registry for hash_table
    test_suite1 = CU_add_suite("Functions", 0, 0);
    if (NULL == test_suite1) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        (NULL == CU_add_test(test_suite1, "Create and destroy database", test_create_destroy_database)) ||
        (NULL == CU_add_test(test_suite1, "Add and remove merch", test_add_remove_merch)) ||
        (NULL == CU_add_test(test_suite1, "Edit merch", test_edit_merch)) ||
        (NULL == CU_add_test(test_suite1, "Replenish merch", test_add_stock)) ||
        (NULL == CU_add_test(test_suite1, "Add to cart", test_add_to_cart)) ||
        (NULL == CU_add_test(test_suite1, "Remove from cart", test_remove_from_cart)) ||
        (NULL == CU_add_test(test_suite1, "Calculate cost", test_calculate_cost)) ||
        (NULL == CU_add_test(test_suite1, "Check out", test_check_out)) 
    )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
} 