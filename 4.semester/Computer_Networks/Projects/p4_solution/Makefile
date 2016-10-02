#Author: Igor Tryhub, 275235

CC = g++
FLAGS = -Wall -W -g -Wreturn-type -std=c++11 -Wundef -pedantic -pedantic-errors

server: Makefile main.o setup.o converse.o parse.o build_response.o
	$(CC) $(FLAGS) -o server main.o setup.o converse.o parse.o build_response.o
	
main.o: main.cpp main.h
	$(CC) -c $(FLAGS) main.cpp -o main.o 
	
setup.o: setup.cpp setup.h
	$(CC) -c $(FLAGS) setup.cpp -o setup.o 
	
converse.o: converse.cpp converse.h parse.h build_response.h
	$(CC) -c $(FLAGS) converse.cpp -o converse.o 
	
parse.o: parse.cpp parse.h converse.h
	$(CC) -c $(FLAGS) parse.cpp -o parse.o 
	
build_response.o: build_response.cpp build_response.h converse.h
	$(CC) -c $(FLAGS) build_response.cpp -o build_response.o 
	
cleanall:
	rm *.o server
	
clean:
	rm *.o
	
.PHONY: cleanall clean


	
