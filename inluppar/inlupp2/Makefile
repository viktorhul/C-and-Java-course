test:
	@gcc -ggdb -Wall -pedantic -std=c11 inlupp1_ref/hash_table.c inlupp1_ref/linked_list.c inlupp1_ref/list_iterator.c business_logic.c generic_utils.c frontend.c tests.c -o tests -lcunit -lm -D_XOPEN_SOURCE=700
	@valgrind --leak-check=full ./tests

gcov:
	@gcc -fprofile-arcs -ftest-coverage -ggdb -Wall -pedantic  -std=c11 inlupp1_ref/hash_table.c inlupp1_ref/linked_list.c inlupp1_ref/list_iterator.c business_logic.c generic_utils.c frontend.c tests.c -o test_gcov -lcunit -lm -D_XOPEN_SOURCE=700
	@gcov ./test_gcov

prog: 
	@gcc -ggdb -Wall -pedantic -std=c11 inlupp1_ref/hash_table.c inlupp1_ref/linked_list.c inlupp1_ref/list_iterator.c business_logic.c generic_utils.c frontend.c main.c -o prog -lcunit -lm -D_XOPEN_SOURCE=700
	@echo "Type ./prog to run"

valgrind_prog:
	rm -f prog
	make prog
	valgrind --leak-check=full ./prog < text_input.txt

cleanup:
	rm *.gcov *.gcno