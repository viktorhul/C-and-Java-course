#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool is_number(char *str) {
    // Check if negative number
    if (str[0] != '-' && !isdigit(str[0])) {
        return false;
    }
    
    // Loop through the remainder of the string
    for (int i = 1; i < strlen(str); i++) {
        if (!isdigit(str[i])) return false;
    }

    // If no non-numeric values were found, return true
    return true;
}

int main(int argc, char *argv[]) {
    if (argc > 1 && is_number(argv[1])) {
        printf("%s is a number\n", argv[1]);
    } else {
        if (argc > 1) {
            printf("%s is not a number\n", argv[1]);
        } else {
            printf("Please provide a command line argument!\n");
        }
    }
}