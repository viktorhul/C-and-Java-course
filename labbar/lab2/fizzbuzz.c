#include <stdio.h>
#include <stdlib.h>

void print_number(int num);

int main(int argc, char *argv[])
{
    int a = atoi(argv[1]);
    
    for (int i = 1; i <= a; i++)
    {
        print_number(i);
        if(a!=i){
            printf(", ");
        }else{
            puts("");
        }
    }
    
  return 0;
}

void print_number(int num){
    if(((num % 3)==0) && ((num % 5)==0)){
            printf("Fizz Buzz");
        }else if ((num % 3)==0){
            printf("Fizz");
        }else if((num % 5)==0){ 
            printf("Buzz");
        }else{
            printf("%d",num);
        }
       
}