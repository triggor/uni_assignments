//Author: Igor Tryhub, 275235
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

uint32_t mask = 0x55555555;
bool passes(uint32_t x);

int main(int argc, char *argv[])
{
  if (argc!=3) {
    printf ("Niepoprawne uzycie. Prosze podac `min` i `max` przedzialu jako argumenty.\n");
    return 1;
  }
  uint32_t min = (uint32_t)atoi(argv[1]), max = (uint32_t)atoi(argv[2]);
  for (uint32_t i=min; i<=max; i++){
    if (passes(i)) printf("%u  ", i);
  }
  printf("\n");
  return 0;
}

bool passes(uint32_t x) {
  if (mask & x){
    if (!((mask<<1) & x)){
      return true;
    }
    else return false;
  }
  if ((mask<<1) & x) {
    return true;
  }
  return false;
}
