//Author: Igor Tryhub,275235
#include <stdio.h>
#include <stdlib.h>
/*
typedef struct matrix {
  int m [2][2];
} Matrix;
*/


unsigned fibonacci_rec (unsigned n);
unsigned fibonacci_iter (unsigned n);
/*unsigned fibonacci_log (unsigned n);
Matrix matrix_mult (Matrix a, Matrix b);
*/
int main ()
{
    unsigned n;
    while (1){
      scanf("%u", &n);
      printf("%u\n", fibonacci_iter(n));
      printf("%u\n", fibonacci_rec(n));
    }
    /*
    Matrix a, b;
    a.m[0][0]=1;
    a.m[0][1]=1;
    a.m[1][0]=1;
    a.m[1][1]=0;
    b.m[0][0]=1;
    b.m[0][1]=1;
    b.m[1][0]=1;
    b.m[1][1]=0;
    printf ("{{%d,%d},{%d,%d}}\n", a.m[0][0], a.m[0][1], a.m[1][0], a.m[1][1]);
    printf ("{{%d,%d},{%d,%d}}\n", b.m[0][0], b.m[0][1], b.m[1][0], b.m[1][1]);
    Matrix c = matrix_mult (a, b);
    printf ("{{%d,%d},{%d,%d}}\n", c.m[0][0], c.m[0][1], c.m[1][0], c.m[1][1]);
    Matrix d = matrix_mult (c, c);
    printf ("{{%d,%d},{%d,%d}}\n", d.m[0][0], d.m[0][1], d.m[1][0], d.m[1][1]);
*/
    return 0;
}

unsigned fibonacci_rec (unsigned n) {
  if (n==0) return 0;
  if (n==1) return 1;
  return fibonacci_rec(n-1) + fibonacci_rec(n-2);
}
unsigned fibonacci_iter (unsigned n) {
  if (n==0) return 0;
  if (n==1) return 1;
  unsigned fib[3] = {0,1};
  while (n>1){
    fib[2] = fib[0]+fib[1];
    fib[0] = fib[1];
    fib[1] = fib[2];
    n--;
  }
  return fib[2];
}
/*
unsigned fibonacci_log (unsigned n) {
  unsigned fib[40] = {0};
  int counter = 1;
  fib[0] = fibonacci(0);
  while (counter <= n) {
    fib[counter] = fibonacci(counter);
    counter *= 2;
  }
  if (counter==n) return fib[counter];
  counter = 1;
  unsigned res = 0;
  if (n%2){
    res = fib[0];
  }
  while (n > 0) {
    n /= 2;
    res += n%2==0 ? 0 : fib[counter];
    counter *= 2;
  }
  return res;
}

Matrix matrix_mult (Matrix a, Matrix b) {
  Matrix res;
  res.m[0][0]=a.m[0][0]*b.m[0][0]+a.m[0][1]*b.m[1][0];
  res.m[0][1]=a.m[0][0]*b.m[0][1]+a.m[0][1]*b.m[1][1];
  res.m[1][0]=a.m[1][0]*b.m[0][0]+a.m[1][1]*b.m[1][0];
  res.m[1][1]=a.m[1][0]*b.m[0][1]+a.m[1][1]*b.m[1][1];
  return res;
}
*/
