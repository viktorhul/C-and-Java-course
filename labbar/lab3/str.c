#include <stdio.h>
#include <stdlib.h>

typedef int(*int_fold_func)(int, int);
int foldl_int_int(int numbers[], int numbers_siz, int_fold_func f);
int add(int a, int b);

int foldl_int_int(int numbers[], int numbers_siz, int_fold_func f) {
    int result = 0;
    for (int i = 0; i < numbers_siz; ++i) {
        result = f(result, numbers[i]);
    }
    return result;
}

int add(int a, int b) {
    return a + b;
}

long sum(int numbers[], int numbers_siz) {
    return foldl_int_int(numbers, numbers_siz, add);
}

int main() {
    int numbers[] = {1,2,3,4,5};
    int result = sum(numbers, 5);
    printf("%d\n", result);
    return 0;
} 


// ----------------------------------------
 /*
#include <stdio.h>

typedef int(*mathematical_operation)(int, int);

int sum(int a, int b) {
return a + b;
}

int subtract(int a, int b) {
return a - b;
}

//funktion
int perform_mo(int a, int b, mathematical_operation func, mathematical_operation func2) {
    //printf("sf");
    return func(a, b) + func2(b, a);
}

int main() {

int result = perform_mo(4, 3, sum, subtract);
    printf("%d\n", result);
    return 0;
} 


/// Den intressanta delen av programmet
int fib(int num)
{
  int ppf = 0; // the two given fib values
  int pf  = 1;

  for (int i = 1; i < num; ++i)
  {
    int tmp = pf;
    pf = ppf + pf;
    ppf = tmp;
  }

  return pf;
}


int fib_rec(int num) {
    if (num == 1) return 1;
    if (num == 0) return 0;
    
    return fib_rec(num-1) + fib_rec(num-2);
}

/// Den ointressanta main()-funktionen
int main(int argc, char *argv[])
{

age_t jonas_mental_age = 3;
  
  if (argc != 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    int n = atoi(argv[1]);
    if (n < 2)
    {
      printf("fib(%d) = %d\n", n, n);
    }
    else
    {
      printf("fib_rec(%s) = %d\n", argv[1], fib_rec(n));
      printf("fib(%s) = %d\n", argv[1], fib(n));

    }
  }
  return 0;
}
*/