#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*-------------------------------------------------*/

extern char *strdup(const char *);

/*-------------------------------------------------*/

typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
} answer_t;

/*-------------------------------------------------*/

typedef bool(*check_func)(char*);
typedef answer_t(*convert_func)(char*);

/*-------------------------------------------------*/


int read_string(char *buf, int buf_siz);

bool is_number(char *str);
int ask_question_int(char *question);
char *ask_question_string(char *question);
double ask_question_float(char *question);
char println(char *str);
answer_t make_float(char *str);
answer_t ask_question(char *question, check_func check, convert_func convert);
answer_t string_to_int(char *str);
bool not_empty(char *str);
void print(char *str);
bool is_shelf (char *str);
char *ask_question_shelf(char *question);
bool is_number_db (char *str);


#endif