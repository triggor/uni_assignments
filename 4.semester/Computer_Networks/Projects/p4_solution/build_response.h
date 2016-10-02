//Author: Igor Tryhub, 275235
#ifndef BUILD_RESPONSE_H
#define BUILD_RESPONSE_H 1

#include<iostream>
#include<string>
#include <fstream>
#include <cstring>
#include <sys/stat.h>

std::string build_response(const char* path, int* msgsize, bool not_implemented, bool forbidden, bool not_found,
			   bool moved_permanently, bool binary, int extension);
void copy_file(const char* path, char* memblock, bool binary);

#endif