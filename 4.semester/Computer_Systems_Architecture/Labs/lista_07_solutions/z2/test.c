#include <stdio.h>
#include <stdlib.h>
 
typedef struct {
  unsigned long lcm;
  unsigned long gcd;
}result_t;

result_t lcm_gcd(unsigned long, unsigned long);
 
int main(void)
{
  unsigned long a, b;
  scanf("%lu %lu", &a, &b);
  result_t res = lcm_gcd(a, b);
  printf("lcm(%lu, %lu) = %lu; gcd(%lu, %lu) = %lu\n", a, b, res.lcm, a, b, res.gcd);     
  return 0;
} 
