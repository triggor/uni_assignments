#Author: Igor Tryhub, 275235

CC = gcc
FLAGS = -std=gnu99 -Wall -Wextra

zombie: Makefile zombie.o
	$(CC) $(FLAGS) -o zombie zombie.o
	
zombie.o: zombie.c
	$(CC) -c $(FLAGS) zombie.c -o zombie.o 
	
orphan: Makefile orphan.o
	$(CC) $(FLAGS) -o orphan orphan.o
	
orphan.o: orphan.c
	$(CC) -c $(FLAGS) orphan.c -o orphan.o 
	
clean:
	rm *.o zombie orphan
	
.PHONY: clean


	
