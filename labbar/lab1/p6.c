#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

// Find the greatest common divider
int gcd(int a, int b) {
    if (a == b) {
        return a;
    }
    else if (a > b) {
        gcd(a - b, b);
        
    } else if (a < b) {
        gcd(a, b - a);
    }
}

int main(int argc, char *argv[]) {
    // Check if correct amount of arguments
    if (argc != 3) {
        printf("Need exactly two arguments with positive integers\n");
        return 0;
    }

    //kontroll 
    if (!is_number(argv[1]) || !is_number(argv[2])) {
        printf("The arguments need to be integers.\n");
        return 0;
    }
    
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    
    if (a < 0 || b < 0) {
        printf("The arguments need to be positive integers.\n");
        return 0;
    }
    
    int common_divider = gcd(a, b);
    
    printf("%d is the greatest common divider\n", common_divider);
    return 0;
}