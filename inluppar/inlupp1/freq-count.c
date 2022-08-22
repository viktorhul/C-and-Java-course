#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "iterator.h"
#include "hash_table.h"
#include "linked_list.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

int cmpstringp(const void *p1, const void *p2) {
  return strcmp(
    *(char *const *) p1, 
    *(char *const *) p2
    );
}

static bool string_eq(elem_t e1, elem_t e2) {
    return (strcmp(e1.p, e2.p) == 0);
}

void sort_keys(char *keys[], size_t no_keys) {
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht) {
  bool has_word = ioopm_hash_table_has_key(ht, ptr_elem(word));
  int freq = has_word ? ioopm_hash_table_lookup(ht, ptr_elem(word)).value.i : 0;
  char *str = has_word ? word : strdup(word);

  ioopm_hash_table_insert(ht, ptr_elem(str), int_elem(freq + 1));
}

void process_file(char *filename, ioopm_hash_table_t *ht) {
  FILE *f = fopen(filename, "r");

  while (true) {
    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, f);

    if (feof(f)) {
      free(buf);
      break;
    }

    for (char *word = strtok(buf, Delimiters);
         word && *word;
         word = strtok(NULL, Delimiters)) {
      process_word(word, ht);
    }

    free(buf);
  }

  fclose(f);
}

int string_sum_hash(elem_t e) {
  char *str = e.p;
  int result = 0;

  do {
    result += *str;
  } while (*++str != '\0');

  return result;
}

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    puts("Usage: freq-count file1 ... filen");
    return 0;
  }

  ioopm_hash_table_t *ht = ioopm_hash_table_create((ioopm_hash_function) string_sum_hash, string_eq, NULL);

  for (int i = 1; i < argc; ++i) {
    process_file(argv[i], ht);
  }

  ioopm_list_t *keys = ioopm_hash_table_keys(ht);

  int size = ioopm_hash_table_size(ht);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(keys);

  char *keys_list[size];
  link_t *key = keys->head;
  int n = 0;

  keys_list[n++] = key->value.p;
  while (ioopm_iterator_has_next(iter)) {
      keys_list[n++] = ioopm_iterator_next(iter).value.p;
  }
  
  sort_keys(keys_list, size);

  char *ptrs[size];

  for (int i = 0; i < size; ++i) {
    int freq = (ioopm_hash_table_lookup(ht, ptr_elem(keys_list[i]))).value.i;
    printf("%s: %d\n", keys_list[i], freq);
    ptrs[i] = keys_list[i];
  }

  for (int i = 0; i < size; i++) 
    free(ptrs[i]);

  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(keys);
  ioopm_hash_table_destroy(ht);
}
