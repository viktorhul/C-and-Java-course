#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    if (argc == 1 || argc > 3) { //can't be 0 args
        printf("Usage: ./a.out rows growth\n");
        return 0;
    }
    int totalStar=0;
    int row = atoi(argv[1]);
    int inc = atoi(argv[2]);
    for (int i = 1; i <= row; i++) {
        for (int j = 1; j <= inc * i; j++) {
            printf("*");
            totalStar++;
        }    
        printf("\n");
    }
    printf("Totalt: %d\n", totalStar);
    return 0;
}
