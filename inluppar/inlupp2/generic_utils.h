#include "inlupp1_ref/common.h"
#include "common.h"
/**
 * @file generic_utils.h
 * @author Viktor Hultsten & Patricia Müller
 * @date 4 Nov 2021
 * @brief Generic util functions that are not specific to the program
 * 
 */

// Union of different data types in return value
typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
} answer_t;

// Specification of valid format, i.e. integer range or chars
typedef union valid_format valid_format_t;

// Uses check function to return if true
typedef bool(*check_func)(char*);

// Converts and returns an answer_t
typedef answer_t(*convert_func)(char*);

/// @brief appends an element to a list
/// @param elem to append to the list
/// @param array to be appended on
/// @param new_size number of elements after appending
/// @returns the new list
char **append_array(char *elem, char **array, int new_size);

/// @brief checks if string is a valid number
/// @param str string to check
/// @returns true if it is a valid number
bool is_valid_number(char *str);

/// @brief checks if string is a valid character (and strlen == 1)
/// @param str string to check
/// @returns true if it is a valid character
bool is_valid_char(char *str);

/// @brief checks if string is a valid shelf format
/// @param str string to check
/// @returns true if it is a valid shelf format
bool is_valid_shelf(char *str);

/// @brief checks if a character is present in a string
/// @param c character to check for presence
/// @param str string to check inside
/// @returns true if it is a valid number
bool char_in_string(char c, char *str);

/// @brief hashing function for hash table keys
/// @param key key to hash
/// @returns the hash
unsigned long ioopm_string_sum_hash(const elem_t key);

/// @brief checks if two values are equal, given that they are integers
/// @param key first argument
/// @param value_ignored N/A
/// @param arg1 second argument
/// @returns if the arguments are equal
bool ioopm_int_eq(elem_t key, elem_t value_ignored, elem_t arg1);

/// @brief checks if two values are equal, given that they are strings
/// @param key_ignored N/A
/// @param value first argument
/// @param arg1 second argument
/// @returns if the arguments are equal
bool ioopm_string_eq(elem_t key_ignored, elem_t value, elem_t arg1);

/// @brief checks if two values are equal, given that they are strings
/// @param key first argument
/// @param value_ignored N/A
/// @param arg1 second argument
/// @returns if the arguments are equal
bool ioopm_string_key_eq(elem_t key, elem_t value_ignored, elem_t arg1);

/// @brief reads a user input
/// @param buf buffer to store the input in
/// @param buf_siz max size of input that will be read
/// @returns string length of input
int ioopm_read_string(char *buf, int buf_siz);

/// @brief compares two strings
/// @param p1 first string
/// @param p2 second string
/// @returns 0 if strings are equal
int ioopm_cmpstringp(const void *p1, const void *p2);

/// @brief sorts array elements
/// @param keys array to sort
/// @param no_keys number of elements
void ioopm_sort_keys(char *keys[], size_t no_keys);

/// @brief modifies char to first uppercase, rest lowercase
/// @param value pointer to string to modify
/// @returns pointer to modified string
elem_t ioopm_first_char_upper(elem_t value);

/// @brief finds next prime
/// @param current_buckets the prime before
/// @returns next prime
size_t ioopm_nextprime(size_t current_buckets);

/// @brief reads user input based on the requirements
/// @param input_type type of input, i.e. 1 for int range
/// @param valid_options specification of input, i.e. int range 1 to 10
/// @param result pointer to user input
/// @returns true if input is valid
bool read_input(int input_type, valid_format_t valid_options, void *result);