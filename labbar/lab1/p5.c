#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Too many arguments\n");
        return 0;
    }

    int prime = atoi(argv[1]);
    int prime_limit = floor(sqrt(prime)) + 1;

    if (prime < 2) {
        printf("Not a prime number (less than two)\n");
        return 0;
    }
    
    for (int x = 0; x < prime_limit; x++) {
        for (int y = 0; y < prime; y++) {
            if (x * y == prime) {
                printf("Not a prime number: %d * %d = %d\n", x, y, prime);
                return 0;
            }
        }
        
    }
    printf("%d is a Prime number\n",prime);
    return 0;
}