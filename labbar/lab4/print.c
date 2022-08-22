#include <stdio.h>

void print(char *str) {
    while (putchar(*str++));
}


int main(void) {
    print("This is a string\n");
    return 0;
}