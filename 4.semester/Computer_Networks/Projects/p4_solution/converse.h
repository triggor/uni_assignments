//Author: Igor Tryhub, 275235
#ifndef CONVERSE_H
#define CONVERSE_H 1

#define BACKLOG 100
#define BUFSIZE 2000
#define PATHSIZE 300
#define HOSTSIZE 100
#define MSGSIZE 200000
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>


int converse(int sockfd, char* server_ip);

#endif