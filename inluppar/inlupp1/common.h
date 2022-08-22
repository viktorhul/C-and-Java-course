#pragma once


typedef union elem elem_t;
union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  /// other choices certainly possible
};

//A function that checks a certain predicate.
typedef bool(*ioopm_predicate)(elem_t key, elem_t value, void *extra);
// Applies function extra to key or value
typedef void(*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);
// Hashes an elem_t and returns the hashed int
typedef int(*ioopm_hash_function)(elem_t key);
// Compares two elem_t and returns a boolean
typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

// Returns the value (if there is one) and a success/failure indication. If the operation succeeds we return a bool true as well as the value. Otherwise we return the bool as false.
typedef struct option option_t;
struct option {
  bool success;
  elem_t value;
};

// Macros to help raise the abstraction level

// int_elem to treat x as int
// ptr_elem to treat x as pointer
#define int_elem(x) (elem_t) { .i=(x) }
#define ptr_elem(x) (elem_t) { .p=(x) }

// Success and Failure creates option values
// Successful and Unsuccessful inspects the option value
#define Success(v) (option_t) { .success = true, .value = v };
#define Failure() (option_t) { .success = false };
#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)