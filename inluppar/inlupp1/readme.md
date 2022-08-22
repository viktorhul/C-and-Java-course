## Instructions to build and run tests
1. To test hash table: make test_hash_table
2. To test linked list: make test_linked_list
3. To run valgrind: make valgrind_hash_table and make valgrind_linked_list

# Assumptions and general info
Keys are treated as integers and values are treated as strings as default.
Add local compare functions if you choose other data types.

Errors are handled by creating a new structure that returns the value (if there is one) and a success/failure indication. If the operation succeeds we return a bool true as well as the value. Otherwise we return the bool as false.
The hash table does not take ownership of the data of its keys and values

## Initial Profiling Results

# For each input, what are the top 3 functions?
1. ioopm_linked_list_append
2. int
3. calloc

# What are the top 3 functions in your code?
1. ioopm_linked_list_append
2. link_create
3. ioopm_iterator_next

# Are the top 3 functions in your code consistent across the inputs? Why? Why not?
It's consistent with shorter words.
If the words are very long like in 1-long-words file then a lot more time will be spent using the hashing function(at least the one that was given to us in freq-count).

# Is there some kind of trend?
The append function uses up a majority of the time, the more words the more % of the time it will spend there.

# Do the results correspond with your expectations?
The program was not perfect, as expected.

# Based on these results, do you see a way to make your program go faster?
- Change append to prepend in hash table insert function, right now we have to iterate through the entire list every time to the end to append.
- Remove hashing in find_previous_entry_for_key. We hash both values to make them comparable instead of using the already given key compare function.

# Time consumption (real, user, sys)
10k words: (0.2s, 0.2s, 0s)
16k words: (30.9s, 30.9s, 0.005s)