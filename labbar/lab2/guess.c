//#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "utils.c"
#include <time.h>

int main(){
    int guess;
    int buf_siz = 5;
    char N[buf_siz];

    srand(time(NULL));
    int T = rand() % 300;
        
    ask_question_string("Skriv ditt namn", N, buf_siz);
    
    char questionInt[300];
    snprintf(questionInt, sizeof(questionInt), "Du, %s, jag tänker på ett tal... kan du gissa vilket?", N);

    guess = ask_question_int(questionInt);
    for (int guessCount = 0; guessCount < 15; guessCount++) {   
        if (guess == T){
            printf("Bingo!\n");
            printf("Det tog %s %d gissningar att komma fram till %d\n", N, guessCount, T);
            return 0;
        } else if (guess > T) {
            printf("För stort! ");
        } else {
            printf("För litet! ");
        }
        guess = ask_question_int("Försök igen");
    }
    printf("Nu har du slut på gissningar! Jag tänkte på %d\n", T);
    return 0;
}