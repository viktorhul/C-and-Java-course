#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <string.h>

int ask_question_int(char *question)
{
    int result = 0;
    int conversions = 0;
    do{
        printf("%s\n", question);
        conversions = scanf("%d", &result);
        clear_input_buffer();
        putchar('\n');
    } while (conversions < 1);
  return result;
}

char *ask_question_string(char *question, char *buf, int buf_siz) {
    do {
        printf("%s: ", question);
    } while (!read_string(buf, buf_siz));
    
    return strdup(buf);
}

void clear_input_buffer()
{
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