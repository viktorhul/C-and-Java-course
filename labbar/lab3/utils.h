#pragma once
#include <stdbool.h>

// Typedef och annat tjafs
extern char *strdup(const char *);

typedef union { 
    int   int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool(*check_func)(char *answer);
typedef answer_t(*convert_func)(char *answer);

// Functions
answer_t ask_question(char *question, check_func check, convert_func convert);
bool not_empty(char *str);
char *ask_question_string(char *question);
bool is_float(char *str);
answer_t make_float(char *str);
double ask_question_float(char *question);
bool is_number(char *str);
int ask_question_int(char *question);
void clear_input_buffer();
int read_string(char *buf, int buf_siz);
void print(char *str);
void println(char *str);
int string_length(char *str);