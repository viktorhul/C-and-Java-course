#include <stdio.h>
#include <stdlib.h>   
    
int main(){
    for (int i = 10; i >=1; i--) {
        printf("%d\n",i);
    }
    
    int i = 10;
    while (i >= 1)            // iterationsvillkor (utför blocket så länge i är mindre än 11)
    {                          // loop-kropp (utförs så länge iterationsvillkoret är uppfyllt)
        printf("%d\n", i);     // skriv ut 1, och en radbrytning
        i = i - 1;             // öka i:s värde med 1 (förändring av iterationsvariabeln)
    }

    return 0;
}