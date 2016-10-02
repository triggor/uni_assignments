//Author: Igor Tryhub, 275235
#include "setup.h"

int setup (char * server_ip) {
  int status;
  struct addrinfo hints;
  struct addrinfo *res;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if((status = getaddrinfo(server_ip, MYPORT, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }
  if (res->ai_next != NULL) fprintf(stderr, "res->ai_next != NULL\n");

  char ipstr[INET_ADDRSTRLEN];
  struct sockaddr_in *ipv4 = (struct sockaddr_in *) res->ai_addr;
  inet_ntop(res->ai_family, &(ipv4->sin_addr), ipstr, INET_ADDRSTRLEN);
  int sockfd;
  if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
    perror("socket error");
    exit(1);
  }

  int yes = 1;
  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1){
    //Allows the socket to be bound to an address that is already in use.
    perror("setsockopt");
    exit(1);
  }
  
  if((status = bind(sockfd, res->ai_addr, res->ai_addrlen)) != 0) {
    perror("bind error");
    exit(1);
  }

  freeaddrinfo(res);

  return sockfd;
}