#include <stdio.h>
#include <stdlib.h>   
    
int main() {
    int totalStar = 0;
    for (int i = 1; i <= 10; i++) {
        for (int j = 0; j < i; j++) {
            printf("*");
            totalStar++;
        }
        printf("\n");
    }
    printf("Totalt: %d\n", totalStar);
    return 0;
}