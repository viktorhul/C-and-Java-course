# How to build the program
Run the make file with *make prog* to compile 

# How to run the program
Use *./prog* to run the program


# Design decisions
* The user interface's current design demonstrates the possible choices that can be done in a simple webstore, wether it's a store manager or a user.
* Current user cannot create or remove a shopping cart on their own. This is done automatically when the first item is being added to a shopping cart and at check out. 
* Valid inputs are being tested in UI, hence there are no tests for valid inputs in business_logic.c
* Errors are handled by user messages and TEXT_ALERT
* Functions use in this program are part from Viktor Hultstens utils.h and Patricia Müllers hash_table.h & linked_list.h


# code coverage 
* File 'business_logic.c'
Lines executed:89.26% of 242
Creating 'business_logic.c.gcov'

* File 'generic_utils.c'
Lines executed:4.92% of 122
Creating 'generic_utils.c.gcov'