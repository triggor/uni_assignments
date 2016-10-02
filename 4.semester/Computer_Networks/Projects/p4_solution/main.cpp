//Author: Igor Tryhub, 275235
#include "main.h"

int main (void){
  
  std::cout << "\n                              *****Server v1.0*****\n\n";
  int sockfd [4];
  char server_ip [4][INET_ADDRSTRLEN] = {"127.10.10.1",
					 "127.10.10.2",
					 "127.10.10.3",
					 "127.10.10.4"};
  
  //Opening 4 sockets, one for each IP-address
  for(int i = 0; i < 4; i++){
    sockfd[i] = setup(server_ip[i]);
  }
  
  pid_t pid;
  for(int i = 0; i < 4; i++){	
    pid = fork();
    if (pid < 0){
      perror("fork error");
      for(int j = 0; j < 4; j++){
	close(sockfd[j]);
      }
      exit(1);
    }
    else if(pid == 0){//Child process
      converse(sockfd[i], server_ip[i]);
      break;
    }
  }
  
  while(pid > 0){
    usleep(5000000);
    continue;
  }
  

  for(int i = 0; i < 4; i++){
    close(sockfd[i]);
  }
  
  return 0;
}

