//Author: Igor Tryhub, 275235
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/prctl.h>


int main (/*int argc, char** argv*/) {
   /*if (argc !=2) {
    printf("Niepoprawne uzycie: %s 'yes/no'.\n", argv[0]);
    exit(1);
  }
 if (strcmp(argv[1], "no") == 0){ // eliminate zombie
    
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGCHLD, &sa, 0) == -1) {
      perror(0);
      exit(1);
    }
  }
  */
  int pid = fork();
  if (pid == 0) { // child
    long int reaper;
    if (prctl(PR_GET_CHILD_SUBREAPER, (int*)&reaper, 0, 0, 0) == -1) {
      perror(0);
      _exit(1);
    }
    printf("Process reaper before: %ld\n", reaper);
    
    sleep(10);
     if (prctl(PR_GET_CHILD_SUBREAPER, (int*)&reaper, 0, 0, 0) == -1) {
      perror(0);
      _exit(1);
    }
    printf("Process reaper after: %ld\n", reaper);
    _exit(0);
  }
  else { //parent
    sleep(5);
  }
  return 0;
}