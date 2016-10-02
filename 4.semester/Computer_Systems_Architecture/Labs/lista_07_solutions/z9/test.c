#include <stdio.h>
#include <stdlib.h>
 
long adds(long x, long y)
{
  asm(
    
    "mov %rdi, %rcx\n"
    "add %rsi, %rcx\n"
    "jb CARRY\n"
    "mov %rcx, %rax\n"
    "jmp RETURN\n"
    
    "CARRY:\n"
    "js SMALLER\n"
    "mov $0x7FFFFFFFFFFFFFFF, %rax\n"
    "jmp RETURN\n"
    
    "SMALLER:\n"
    "mov $0x8000000000000000, %rax\n"
    
    "RETURN:\n"
    "rep \n"
    
  );
}
 
int main()
{
  long x, y;
  scanf("%ld %ld", &x, &y);
  long res = adds(x, y);
  printf("adds(%ld, %ld) = %ld\n", x, y, res);
  return 0;
}