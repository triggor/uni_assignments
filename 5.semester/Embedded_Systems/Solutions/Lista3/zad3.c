//Author: Igor Tryhub, 275235
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

const uint32_t m1  = 0x55555555;
const uint32_t m2  = 0x33333333;
const uint32_t m4  = 0x0f0f0f0f;
const uint32_t m8  = 0x00ff00ff;
const uint32_t m16 = 0x0000ffff;

bool popcount_more_than_2(uint32_t x);

int main(int argc, char *argv[])
{
  if (argc!=3) {
    printf ("Niepoprawne uzycie. Prosze podac `min` i `max` przedzialu jako argumenty.\n");
    return 1;
  }
  uint32_t min = (uint32_t)atoi(argv[1]), max = (uint32_t)atoi(argv[2]);
  for (uint32_t i=min; i<=max; i++){
    if (popcount_more_than_2(i)) printf("%u  ", i);
  }
  printf("\n");
  return 0;
}

bool popcount_more_than_2(uint32_t x) {
    uint32_t x1, x2;

    x1 = (x & m1 );
    x2 = ((x >>  1) & m1 );
    if (x1 && x2) return true;
    x = x1 + x2;

    x1 = (x & m2 );
    x2 = ((x >>  2) & m2 );
    if (x1 && x2) return true;
    x = x1 + x2;

    x1 = (x & m4 );
    x2 = ((x >>  4) & m4 );
    if (x1 && x2) return true;
    x = x1 + x2;

    x1 = (x & m8 );
    x2 = ((x >>  8) & m8 );
    if (x1 && x2) return true;
    x = x1 + x2;

    x1 = (x & m16 );
    x2 = ((x >> 16) & m16);
    if (x1 && x2) return true;
    x = x1 + x2; //now x==popcount(x)

    return false;
}
