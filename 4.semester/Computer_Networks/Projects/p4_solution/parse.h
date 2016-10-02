//Author: Igor Tryhub, 275235
#ifndef PARSE_H
#define PARSE_H 1

#include <iostream>
#include <string> 
#include <cstring>
#include <fstream>

int parse(char* server_ip, const char* buf, char* path, char* host, bool* connection, bool* binary, int* extension);

#endif