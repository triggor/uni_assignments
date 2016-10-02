#include <stdio.h>

int clz(long t);

int main(void){
  long t;
  scanf("%ld", &t);
  printf("res=%d\n", clz(t));
  return 0;
}