#Author: Igor Tryhub, 275235

CC = gcc
FLAGS = -std=gnu99 -Wall -Wextra

coroutines: Makefile coroutines.o
	$(CC) $(FLAGS) -o coroutines coroutines.o
	
coroutines.o: coroutines.c
	$(CC) -c $(FLAGS) coroutines.c -o coroutines.o 
	
clean:
	rm *.o coroutines
	
.PHONY: clean


	
