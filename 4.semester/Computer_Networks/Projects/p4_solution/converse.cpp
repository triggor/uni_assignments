//Author: Igor Tryhub, 275235
#include "converse.h"
#include "parse.h"
#include "build_response.h"

int converse(int sockfd, char* server_ip){

  int status;
  if((status = listen(sockfd, BACKLOG)) != 0) {
    perror("listen error");
    close(sockfd);
    exit(1);
  }
  
  struct sockaddr_storage their_addr;
  socklen_t addr_size = sizeof(their_addr);
  int new_fd;
  pid_t pid;
  
  while(true){
    if((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size)) == -1) {
      perror("accept error");
      exit(1);
    }
    pid = fork();
    if (pid < 0){
      perror("fork error");
      close(new_fd);
      exit(1);
    }
    else if(pid == 0) break;
  }
  
  char buf [BUFSIZE];
  char path[PATHSIZE];
  char host[HOSTSIZE];
  char msg[MSGSIZE];
  bool connection;//false on "close", true otherwise
  bool binary;
  int extention;
  bool not_implemented = false;
  bool forbidden = false;
  bool not_found = false;
  bool moved_permanently = false;
  
  if((status = recv(new_fd, buf, sizeof(buf), 0)) == -1){
    perror("recv error");
    close(new_fd);
    exit(1);
  }
  if(status < BUFSIZE) buf[status] = '\0';
  std::cout << "\tRequested on socket #" << sockfd << ": " << std::endl << buf << std::endl << std::endl;
  if((status = parse(server_ip, buf, path, host, &connection, &binary, &extention)) != 0){
    if(status==1) not_implemented = true;
    else if(status==2) forbidden = true;
    else if(status==3) not_found = true;
    else if(status==4) moved_permanently = true;
  }
  const char* location = path;
  int msgsize=0;
  strcpy(msg, strdup(build_response(location, &msgsize, not_implemented, forbidden, not_found, moved_permanently, binary, extention).c_str()));
  
  std::cout << "Required to send " << msgsize << " bytes" << std::endl;
  if ((status = send(new_fd, msg, msgsize, 0)) == -1) {
    perror("send error");
    close(new_fd);
    exit(1);
  }
  std::cout << "Sent " << status << " bytes" << std::endl;
      
  //std::cout << "\tAnswered on socket #" << sockfd << ": " << msg << std::endl << std::endl;
  close(new_fd);
  return 0;
}