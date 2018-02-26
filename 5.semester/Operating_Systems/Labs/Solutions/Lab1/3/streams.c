//Author: Igor Tryhub, 275235
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


void split_str(void);
void remove_notalnum(char* pch);
void print_and_count(char* pch);


int pipefd_0_1[2];
int pipefd_1_2[2];
int pipefd_2_3[2];


#define create_pipe(pipefd) \
    do { if (pipe(pipefd) == -1) { perror("pipe"); exit(EXIT_FAILURE); } } while (0)
      
#define create_fork(cpid) \
    do { cpid = fork(); if (cpid == -1) { perror("fork"); exit(EXIT_FAILURE); } } while (0)

int main(void)
{
/********************************* PROCESS MAIN: BEGIN *********************************************/
  pid_t cpid3;
  char buf;
  create_pipe(pipefd_0_1);
  create_fork(cpid3);
/********************************* PROCESS 3: BEGIN *********************************************/
  if (cpid3 == 0) {    /* Child writes to pipe */
	  pid_t cpid2;
	  create_pipe(pipefd_2_3);
	  create_fork(cpid2);
/********************************* PROCESS 2: BEGIN *********************************************/
		if (cpid2 == 0) {    /* Child reads from pipe */
		  pid_t cpid1;
		  create_pipe(pipefd_1_2);
		  create_fork(cpid1);
/********************************* PROCESS 1: BEGIN *********************************************/
			  if (cpid1 == 0) {    /* Child reads from pipe */
			    close(pipefd_0_1[0]);/* Close unused read end */ 
			    close(pipefd_1_2[0]);/* Close unused read end */
			    close(pipefd_2_3[0]);/* Close inherited parent pipe */
			    close(pipefd_2_3[1]);/* Close inherited parent pipe */
			    split_str();
			    close(pipefd_0_1[1]);/* Close after executed */
			    close(pipefd_1_2[1]);/* Close after executed */
			    _exit(EXIT_SUCCESS);
			  } 
/********************************* PROCESS 1: END *********************************************/
/********************************* PROCESS 2: CONTINUE *********************************************/
		  close(pipefd_0_1[0]);/* Close inherited parent pipe */
		  close(pipefd_0_1[1]);/* Close inherited parent pipe */
		  close(pipefd_1_2[1]);/* Close unused write end */
		  close(pipefd_2_3[0]);/* Close unused read end */
		  char pch[100];
		  int i = 0;
		  while(read(pipefd_1_2[0], &pch[i], 1)){
		    while(pch[i]!=10){
		      i++;
		      read(pipefd_1_2[0], &pch[i], 1);
		    }
		    pch[i]='\0';
		    remove_notalnum(pch);
		    i=0;
		  }
		  close(pipefd_1_2[0]);/* Close after executed */
		  close(pipefd_2_3[1]);/* Close after executed */
		  wait(NULL);                /* Wait for child */
		  _exit(EXIT_SUCCESS);
		  
		} 
/********************************* PROCESS 2: END *********************************************/
/********************************* PROCESS 3: CONTINUE *********************************************/
	            
	  close(pipefd_0_1[0]);/* Close inherited parent pipe */
	  close(pipefd_0_1[1]);/* Close inherited parent pipe */
	  close(pipefd_2_3[1]);/* Close unused write end */
	  char pch[100];
	  int j = 0;
	  while(read(pipefd_2_3[0], &pch[j], 1)){
	    while(pch[j]!=10){
	      j++;
	      read(pipefd_2_3[0], &pch[j], 1);
	    }
	    pch[j]='\0';
	    if (strlen(pch) >= 2 || isalnum(pch[0])){
	      print_and_count(pch);
	    }
	    j=0;
	  }
	  close(pipefd_2_3[0]);/* Close after executed */
	  wait(NULL); 			/* Wait for child */
	  _exit(EXIT_SUCCESS);
	  
  }
/********************************* PROCESS 3: END *********************************************/
/********************************* PROCESS MAIN: CONTINUE *********************************************/           
    close(pipefd_0_1[1]);          /* Close unused write end */
    while (read(pipefd_0_1[0], &buf, 1) > 0)
	write(STDOUT_FILENO, &buf, 1);
    write(STDOUT_FILENO, " (Said by parent)", 17);
    write(STDOUT_FILENO, "\n", 1);
    wait(NULL);                /* Wait for child */
    close(pipefd_0_1[0]);/* Close after executed */
    exit(EXIT_SUCCESS);
/********************************* PROCESS MAIN: END *********************************************/           
}

void split_str(void){
  unsigned count = 0;
  char str[1000];
  char* pch;
  char temp[10];
   
  do {
    fgets (str, 1000, stdin);
    pch = strtok (str," ");
    while (pch!=NULL){
      if (strlen(pch) >= 2 || isalnum(pch[0])) {	
        count++;
	write(pipefd_1_2[1], pch, strlen(pch));
	write(pipefd_1_2[1], "\n", 1);
      }
      pch = strtok (NULL, " ");
    }
  } while (strlen(str) >= 2 || isalnum(str[0]));
  write(pipefd_0_1[1], "Word count: ", 12);
  sprintf(temp, "%u", count);
  write(pipefd_0_1[1], temp, strlen(temp));
}

void remove_notalnum(char* pch){
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
  write(pipefd_2_3[1], pch, strlen(pch));
  write(pipefd_2_3[1], "\n", 1);
}

void print_and_count(char* pch){
  unsigned word_len = strlen(pch);
  printf ("\t%s (%u)\n", pch, word_len);
}









