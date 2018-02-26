//Author: Igor Tryhub, 275235
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void split_str(void);
static void remove_notalnum(void);
static void print_and_count(void);

static ucontext_t uctx_main, uctx_func1, uctx_func2, uctx_func3;
static unsigned count = 0;
static char* pch;

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(void)
{
   char func1_stack[16384];

   if (getcontext(&uctx_func1) == -1)
        handle_error("getcontext");
    uctx_func1.uc_stack.ss_sp = func1_stack;
    uctx_func1.uc_stack.ss_size = sizeof(func1_stack);
    uctx_func1.uc_link = &uctx_main;
    makecontext(&uctx_func1, split_str, 0);

   printf("main: swap(main-->1)\n");
    if (swapcontext(&uctx_main, &uctx_func1) == -1)
        handle_error("swapcontext");

   printf("main: exiting\n");
    exit(EXIT_SUCCESS);
}

static void split_str(void){
  char func2_stack[16384];
  printf("1: started\n");
  char str[1000];
  do {
    printf("new cycle\n");
    fgets (str, 1000, stdin);
    pch = strtok (str," ");
    while (pch!=NULL){
      printf("new word\n");
      if (strlen(pch) >= 2 || isalnum(pch[0])) {
	printf("swap(1-->2)\n");
	
	if (getcontext(&uctx_func2) == -1)
	  handle_error("getcontext");
	uctx_func2.uc_stack.ss_sp = func2_stack;
	uctx_func2.uc_stack.ss_size = sizeof(func2_stack);
	uctx_func2.uc_link = &uctx_func1;
	makecontext(&uctx_func2, remove_notalnum, 0);
	
	if (swapcontext(&uctx_func1, &uctx_func2) == -1)
	  handle_error("swapcontext");
        count++;
      }
      pch = strtok (NULL, " ");
    }
  } while (strlen(str) >= 2 || isalnum(str[0]));
  printf("\nWord count: %u\n", count);
  printf("1: returning\n");
}

static void remove_notalnum(void){
  char func3_stack[16384];
  printf("2: started\n");
  unsigned j=0;
  typedef enum {ALNUM, NOTALNUM} symbol;
  symbol prev = ALNUM;
  symbol curr;
  unsigned move_cnt = 0;
  unsigned beg_move = -1;
  unsigned notalnum_cnt = 0;

  while(pch[j]!='\0'){
    if (isalnum(pch[j])) curr = ALNUM;
    else curr = NOTALNUM;
    if (prev==ALNUM && curr==NOTALNUM){
      beg_move = j;
      move_cnt = 1;
    }
    else if (prev==NOTALNUM && curr==NOTALNUM){
      move_cnt++;
    }
    else if (prev==NOTALNUM && curr==ALNUM){
      memmove(pch+beg_move, pch+j, strlen(pch)-j+1);
      notalnum_cnt += move_cnt;
      j -= move_cnt;
    }
    j++;
    prev=curr;
  }

  if (prev==NOTALNUM) {
    notalnum_cnt += move_cnt;
    memmove(pch+beg_move, pch+j, strlen(pch)-j+1);
  }
  printf("swap(2-->3)\n");
  
  if (getcontext(&uctx_func3) == -1)
      handle_error("getcontext");
  uctx_func3.uc_stack.ss_sp = func3_stack;
  uctx_func3.uc_stack.ss_size = sizeof(func3_stack);
  uctx_func3.uc_link = &uctx_func2;
  makecontext(&uctx_func3, print_and_count, 0);
  
    if (swapcontext(&uctx_func2, &uctx_func3) == -1)
        handle_error("swapcontext");
  printf("2: returning\n");
}

static void print_and_count(void){
  printf("3: started\n");
  unsigned word_len = strlen(pch);
  printf ("\t%s (%u)\n", pch, word_len);
  printf("swap(3-->2)\n");
    if (swapcontext(&uctx_func3, &uctx_func2) == -1)
        handle_error("swapcontext");
  printf("3: returning\n");
}









