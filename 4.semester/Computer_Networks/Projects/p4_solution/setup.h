//Author: Igor Tryhub, 275235
#ifndef SETUP_H
#define SETUP_H 1

#define MYPORT "8888"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int setup (char * server_ip);

#endif