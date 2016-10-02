  .globl insert_sort

#(%rdi)=first; (%rsi)=last; %r8=size; %rax=index; %rdx=elem_i; %r9=elem_j;
  
insert_sort:
  xor %rax,%rax #%rax=0
  mov %rsi,%r8
  sub %rdi,%r8 #%r8=%rsi-%rdi
  shr $3,%r8 #%r8=%r8/8
  test %r8,%r8 
  je return #empty tab
  
loop:
  inc %rax #%rax++
  cmp %rax,%r8 
  je return #last iteration
  mov %rax,%rcx

find_position:
  test %rcx,%rcx
  je loop #position found
  mov (%rdi,%rcx,8),%rdx #current elem
  mov -8(%rdi,%rcx,8),%r9 #prev elem
  cmp %rdx,%r9
  jle loop #current<prev => ok
  mov %r9,(%rdi,%rcx,8)
  mov %rdx,-8(%rdi,%rcx,8) #swap
  dec %rcx
  jmp find_position

return:
  ret
