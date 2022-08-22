#test
compile_hash_table:
	@echo "Compiling started."
	@gcc -c hash_table.c linked_list.c main.c iterator.c 
	@gcc -ggdb -Wall -std=c11 -pedantic hash_table.o linked_list.o iterator.o main.o -o main -lcunit
	@rm hash_table.o linked_list.o main.o
	@echo "Compiling completed. Type ./main to run"

compile_iterator:
	@echo "Compiling started."
	@gcc -c iterator.c linked_list.c
	@gcc -ggdb -Wall -std=c11 -pedantic iterator.o linked_list.o -o iterator -lcunit
	@rm iterator.o linked_list.o
	@echo "Compiling completed. Type ./iterator to run"

compile_freq:
	@echo "Compiling started."
	@gcc -c iterator.c linked_list.c hash_table.c freq-count.c
	@gcc -ggdb -Wall -std=c11 -pedantic iterator.o linked_list.o hash_table.o freq-count.o -o freq -lcunit
	@rm iterator.o linked_list.o hash_table.o freq-count.o
	@echo "Compiling completed. Type ./freq to run"

test_hash_table:
	@echo "Compiling started."
	@gcc -c hash_table.c hash_table_tests.c linked_list.c iterator.c
	@gcc -g -ggdb -Wall -std=c11 -pedantic hash_table.o hash_table_tests.o linked_list.o iterator.o -o hash_table_tests -lcunit
	@rm hash_table.o hash_table_tests.o linked_list.o
	@echo "Compiling completed. Type ./hash_table_tests to run."

test_linked_list:
	@echo "Compiling started."
	@gcc -c linked_list.c linked_list_tests.c iterator.c
	@gcc -g -ggdb -Wall -std=c11 -pedantic linked_list.o linked_list_tests.o iterator.o -o linked_list_tests -lcunit
	@rm linked_list.o linked_list_tests.o iterator.o
	@echo "Compiling completed. Type ./linked_list_tests to run."

valgrind_hash_table:
	make test_hash_table
	valgrind --leak-check=full ./hash_table_tests -g

valgrind_linked_list:
	make test_linked_list
	valgrind --leak-check=full ./linked_list_tests -g
