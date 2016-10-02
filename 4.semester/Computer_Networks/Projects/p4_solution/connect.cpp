#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
//kompiluje sie osobno:
//g++ -Wall -W -g -Wreturn-type -std=c++11 -Wundef -pedantic -pedantic-errors -o client connect.cpp

int client_connect (char * ip_addr);

int main (void){
  std::cout << "\n                              #####Client v1.0#####\n\n";
  int sockfd [4];
  char server_ip [4][INET_ADDRSTRLEN] = {"127.10.10.1",
					 "127.10.10.2",
					 "127.10.10.3",
					 "127.10.10.4"};
  for(int i = 0; i < 4; i++){
      sockfd[i] = client_connect(server_ip[i]);
      std::cout << "connecting to " << server_ip[i] << std::endl;
      usleep(1000000);
  }  
  
  std::cout << std::endl;
  for(int i = 0; i < 4; i++){
    close(sockfd[i]);
  }
  
  return 0;
}

int client_connect (char * ip_addr){
  struct addrinfo *res, hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  int status;
  if((status = getaddrinfo(ip_addr, "8888", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }
  
  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(sockfd, res->ai_addr, res->ai_addrlen);
  char msg[1000] = "GET /strony_www/dom1.abc.pl/public-key.txt HTTP/1.1\nHost: dom3.abc.pl:8888\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate\nConnection: clsehgfc\n";
  if ((status = send(sockfd, msg, strlen(msg), 0)) == -1) {
    perror("send error");
    exit(1);
  }
  
  
  char buf[1000];
  int num = recv(sockfd, buf, sizeof(buf), 0);
  if ( num <= 0 ) {
    printf("Either Connection Closed or Error\n");
  }

  buf[num] = '\0';
  //printf("Client:Message Received From Server -  %s\n",buf);
  std::cout << "Accepted from server: " << buf << std::endl;
  
  return sockfd;
}