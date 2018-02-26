//Author: Igor Tryhub, 275235
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/prctl.h>

 /* POSIX.1-2001 specifies that if the disposition of SIGCHLD is set to SIG_IGN, 
  * then children that terminate do not become zombies and a call to wait() or
  * waitpid() will block until all children have terminated, and then fail with
  * errno set to ECHILD. (The original POSIX standard left the behavior of setting SIGCHLD 
  * to SIG_IGN unspecified. Note that even though the default disposition of 
  * SIGCHLD is "ignore", explicitly setting the disposition to SIG_IGN results 
  * in different treatment of zombie process children.) 
  */

int main (int argc, char** argv) {
  if (argc !=2) {
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
  
  int pid = fork();
  if (pid == 0) { // child
    _exit(0);
  }
  else { //parent
    
    char *newenviron[] = { NULL };
    char *newargv[] = {"/bin/ps", "-e",  "-o", "pid,ppid,stat,cmd", NULL};
    if((execve("/bin/ps", newargv, newenviron))==-1){
      perror("execve");   /* execve() returns only on error */
      exit(EXIT_FAILURE);
    } 
  }
  return 0;
}