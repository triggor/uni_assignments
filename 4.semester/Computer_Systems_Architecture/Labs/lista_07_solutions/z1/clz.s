	.text
	.globl	clz
	.type	clz, @function
clz:
# x=%rdi; n=%rax; t=%rdx
  MIN_INT = $0xFFFFFFFF00000000
  mov $64, %rax
  cmp $0, %rdi
  je return #if (x==0) return 64;
  mov $0, %rax #int n = 0;
  mov %rdi, %rdx #long t=x;
  mov $0xFFFFFFFF00000000, %rcx
  and %rcx, %rdx #long t = x & 0xFFFFFFFF00000000;
  cmp $0, %rdx
  jne .L16
  add $32, %rax
  shl $32, %rdi
.L16:
  mov %rdi, %rdx #long t=x;
  mov $0xFFFF000000000000, %rcx
  and %rcx, %rdx
  cmp $0, %rdx
  jne .L8
  add $16, %rax
  shl $16, %rdi
.L8:
  mov %rdi, %rdx #long t=x;
  mov $0xFF00000000000000, %rcx
  and %rcx, %rdx 
  cmp $0, %rdx
  jne .L4
  add $8, %rax
  shl $8, %rdi
.L4:
  mov %rdi, %rdx #long t=x;
  mov $0xF000000000000000, %rcx
  and %rcx, %rdx 
  cmp $0, %rdx
  jne .L2
  add $4, %rax
  shl $4, %rdi
.L2:
  mov %rdi, %rdx #long t=x;
  mov $0xC000000000000000, %rcx
  and %rcx, %rdx 
  cmp $0, %rdx
  jne .L1
  add $2, %rax
  shl $2, %rdi
.L1:
  mov %rdi, %rdx #long t=x;
  mov $0x8000000000000000, %rcx
  and %rcx, %rdx 
  cmp $0, %rdx
  jne return
  add $1, %rax
return:
  ret