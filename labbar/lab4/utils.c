#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

answer_t ask_question(char *question, check_func check, convert_func convert) {
    int buf_siz = 255;
    char buf[buf_siz];
    
    do {
        printf("%s: ", question);
        read_string(buf, buf_siz);
    } while (!check(buf));

    return convert(buf);
}

bool not_empty(char *str) {
    return strlen(str) > 0;
}

char *ask_question_string(char *question) {
    return ask_question(question, not_empty, (convert_func) strdup).string_value;
}

bool is_float(char *str) {   
    int dot_num = 0;

    if (!(str[0] == '-' || str[0] == '.') && !isdigit(str[0])) {
         return false;
    }

    if (str[0] == '.'){ 
        dot_num++;
    }
    
    for (int i = 1; i < strlen(str); i++) {
        if (str[i] == '.'){ 
            dot_num++;
        } else if (!isdigit(str[i]) ){
            return false;
        }
    }

    if (dot_num != 1) return false;
    else return true;
}

answer_t make_float(char *str) {
    return (answer_t) { .float_value = atof(str) };
}

double ask_question_float(char *question) {
    return ask_question(question, is_float, make_float).float_value;
}

bool is_number(char *str) {    
    if (str[0] != '-' && !isdigit(str[0])) {
        return false;
    }

    for (int i = 1; i < strlen(str); i++) {
        if (!isdigit(str[i])) return false;
    }

    return true;
}

int ask_question_int(char *question) {
    return ask_question(question, (check_func) is_number, (convert_func) atoi).int_value;
}

void clear_input_buffer() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

int read_string(char *buf, int buf_siz) {
    char c;
    int counter = 0;

    for (int i = 0; i < buf_siz; i++) {
        c = getchar();
        buf[counter] = c;
        counter++;

        if (c == EOF || c == '\n') {
            break;
        }
    }
    clear_input_buffer();
    buf[counter-1] = '\0';
    return counter;
}

void print(char *str) {
    int char_num = string_length(str); 
    
    for (int i = 0; i < char_num; i++) {
        putchar(str[i]);
    }
}

void println(char *str) {
    print(str);
    putchar('\n');
}

int string_length(char *str) {
    int i = 0;
    do {
        if (str[i] == '\0') break;
        i++;
    } while (1);
    return i;
}