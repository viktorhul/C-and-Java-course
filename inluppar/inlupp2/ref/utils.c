#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"


int read_string(char *buf, int buf_siz)
{
    int i = 0;
    char c;
    do
    {
        c = getchar();
        buf[i] = c;
        i++;
    } while (c != '\n' && i <= buf_siz);
    buf[i-1] = '\0';

    while (c != '\n' && '\0')
    {
        c = getchar();
    } 
    return strlen(buf);
}

/*-------------------------------------------------*/

bool is_number (char *str)
{
    int l = strlen(str);
    int i = 0;

        while (i <= l-1)
        {
            char k = str[i];
            bool n = isdigit(k);
            if ((n != 1) && !(k == '-' && i == 0))
            {
                printf("%s is not a number\n", str);
                return 0;
            }
            i++;
        }

    printf("%s is a number\n", str);
    return 1;

}

/*-------------------------------------------------*/

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

/*-------------------------------------------------*/


answer_t string_to_int(char *str)
{
    answer_t a;
    a.int_value = atoi(str);
    printf("%d\n", a.int_value);
    return a;
}

/*-------------------------------------------------*/


answer_t ask_question(char *question, check_func check, convert_func convert)
{
    int buf_siz = 15;
    char number[buf_siz];
    answer_t answer;
    do
    {
        printf("%s", question);
        read_string(number, buf_siz);
    } while (!check(number));
    answer = convert(number);
    return answer;
}

/*-------------------------------------------------*/


int ask_question_int(char *question)
{   
    int a;
    a = ask_question(question, is_number_db, (convert_func)atoi).int_value;
    return a;
}

/*-------------------------------------------------*/

answer_t make_float(char *str)
{
  answer_t a;                
  a.float_value = atof(str); 
  return a;                  
}

/*-------------------------------------------------

double ask_question_float(char *question)
{
    return ask_question(question, is_float, make_float).float_value;
}

-------------------------------------------------*/

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func) strdup).string_value;
}

/*-------------------------------------------------*/


void print(char *str)
{
    char *point_string = str;
    while (*point_string != '\0')
    {
        putchar(*point_string);
        point_string++;
    }
    putchar('\n');
}

/*-------------------------------------------------*/

char *ask_question_shelf(char *question)
{
    return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}

/*-------------------------------------------------*/

bool is_shelf (char *str)
{
    int l = strlen(str);
    char first_char = str[0];
    int i = 1;
    if (isalpha(first_char) && strlen(str) > 1)
    {
        while (i <= l-1) 
        {
            if (!isdigit(str[i]))
            {
                return 0;
            }
            i++;
        }
        return 1;
    }
    return 0;
}

/*-------------------------------------------------*/

bool is_number_db (char *str)
{
    int l = strlen(str);
    int i = 0;

        while (i <= l-1)
        {
            char k = str[i];
            bool n = isdigit(k);
            if ((n != 1) || (k == '-' && i == 0))
            {
                printf("Invalid number: %s\n", str);
                return 0;
            }
            i++;
        }

        if (l <=0 )
        {
            return 0;
        }

    printf("%s is a number\n", str);
    return 1;

}

/*-------------------------------------------------*/
