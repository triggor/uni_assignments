//Author: Igor Tryhub, 275235
#include <stdio.h>
#include <stdint.h>

// i-ty bit --> j-ty bit
int main(int argc, char *argv[])
{
  if (argc!=4) {
    printf ("Niepoprawne uzycie. Prosze podac `x`, `i` oraz `j` jako argumenty.\n");
    return 1;
  }
  uint32_t x = (uint32_t)atoi(argv[1]), i = (uint32_t)atoi(argv[2]), j = (uint32_t)atoi(argv[3]);

  uint32_t oldbit = ((1<<i) & x);
  if (oldbit) x |= (1<<j);
  else x &= ~(1<<j);

  printf("%u\n", x);
  return 0;
}
