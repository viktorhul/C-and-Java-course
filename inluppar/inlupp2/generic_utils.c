/*
 * GENERIC UTILITIES
 * Utilities which are not specific to a program.
*/

#include <stdbool.h>
#include <ctype.h>
#include "common.h"
#include "generic_utils.h"

char **append_array(char *elem, char **array, int new_size) {
    char **arr = calloc(new_size, sizeof(char *));

    arr[0] = elem;

    for (int i = 1; i < new_size; i++) {
        arr[i] = array[i - 1];
    }

    free(array);
    return arr;
}

bool is_valid_number(char *str) {
    while (*str != '\0')
        if (!isdigit(*str++))
            return false;

    return true;
}

bool is_valid_char(char *str) {
    return (
        isalpha(str[0]) && 
        '\0' == str[1]
    );
}

bool is_valid_shelf(char *str) {
    return (
        isalpha(str[0]) &&
        isdigit(str[1]) &&
        isdigit(str[2]) &&
        '\0' == str[3]
    );
}

bool char_in_string(char c, char *str) {
    return (NULL != strchr(str, c));
}

unsigned long ioopm_string_sum_hash(const elem_t key) {
    if(key.integer)
    {
        return key.integer;
    }
    else
    {
        unsigned long result = 0;
        char *str = (char *)key.pointer;
        do
        {
            result += *str; /// str is the ASCII value of
        }
        while (*str++ != '\0');
        return result;
    }

}

bool ioopm_int_eq(elem_t key, elem_t value_ignored, elem_t arg1)
{
    if (!key.integer || !arg1.integer)
        return false;

    return (key.integer == arg1.integer);
}

bool ioopm_string_eq(elem_t key_ignored, elem_t value, elem_t arg1) {  
  if (!value.pointer || !arg1.pointer) return false;
  return (strcmp(value.pointer, arg1.pointer) == 0);
}

bool ioopm_string_key_eq(elem_t key, elem_t value_ignored, elem_t arg1) {  
  if (!key.pointer || !arg1.pointer) return false;
  return (strcmp(key.pointer, arg1.pointer) == 0);
}

int ioopm_read_string(char *buf, int buf_siz) {
    int i = 0;
    char c;

    do {
        c = getchar();
        buf[i] = c;
        i++;
    } while (c != '\n' && i <= buf_siz);

    buf[i-1] = '\0';

    while (c != '\n' && '\0')
        c = getchar();

    return strlen(buf);
}

int ioopm_cmpstringp(const void *p1, const void *p2) {
    return strcmp(
      *(char *const *) p1, 
      *(char *const *) p2
    );
}

void ioopm_sort_keys(char *keys[], size_t no_keys) {
  qsort(keys, no_keys, sizeof(char *), ioopm_cmpstringp);
}

elem_t ioopm_first_char_upper(elem_t value) {
    int i = 0;
    char *str = (char *)value.pointer;
    size_t strl = strlen(str);
    char *new_value = {0};
    while (i < strl && (str[i] != '\0' && str[i] != '\n'))
    {
        if (i == 0 && !isspace(str[i]) && islower(str[i]))
        {
            new_value[i] = toupper(str[i]);
            i++;
        }
        else if (i == 0 && !isspace(str[i]) && isupper(str[i]))
        {
            new_value[i] = str[i];
            i++;
        }
        else 
        {
            new_value[i] = tolower(str[i]);
            i++;
        }
    }
    value.pointer = new_value;
    return value;
}

size_t ioopm_nextprime(size_t current_buckets)
{
  size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};
  for (size_t i = 0; i < 10; i++)
  {
    if(current_buckets < primes[i])return primes[i];
  }
  return primes[10];
}

bool read_input(int input_type, valid_format_t valid_options, void *result) {
    printf(": ");
    int buf_siz = 255;
    char buf[buf_siz];
    ioopm_read_string(buf, buf_siz);

    switch (input_type) {
        case INPUT_CHARS: {
            if (!is_valid_char(buf))
                return false;
            
            char input = toupper(buf[0]);
            char *valid_options_str = valid_options.options.pointer;
            bool valid_input = char_in_string(input, valid_options_str);

            if (valid_input) {
                char *result_char = result;
                *result_char = input;
                return true;
            } else {
                return false;
            }
        }

        case INPUT_INT: {
            if (!is_valid_number(buf))
                return false;

            int input = atoi(buf);
            int min = valid_options.range.min;
            int max = valid_options.range.max;

            bool valid_input = (input >= min && input <= max);

            int *result_int = result;

            if (valid_input) {
                *result_int = input;
                return true;
            } else {
                return false;
            }
        }

        case INPUT_TEXT: {
            char *ptr = result;
            int n = 0;

            while ('\0' != buf[n]) {
                ptr[n] = buf[n];
                n++;
            }
            ptr[n] = '\0';

            return (0 != n);
        }

        case INPUT_SHELF: {
            if (!is_valid_shelf(buf))
                return false;

            char *ptr = result;
            ptr[0] = toupper(buf[0]);
            ptr[1] = buf[1];
            ptr[2] = buf[2];
            ptr[3] = buf[3];
            return true;
        }

        default:
            return false;
    }
}