#  .text:
  .globl lcm_gcd
  
#%rdi=a; %rsi=b; %rax=lcm; %rdx=gcd;

lcm_gcd:
  mov %rdi,%rax
  mul %rsi #%rax = a*b

subtract:
  cmp %rsi,%rdi 
  je return #a==b
  ja swapped #a<b
  sub %rdi,%rsi #a-b
  jmp subtract

swapped:
  sub %rsi,%rdi #b-a
  jmp subtract

return:
  div %rdi #%rax=lcm
  mov %rdi,%rdx #%rdx=gcd
  ret 
