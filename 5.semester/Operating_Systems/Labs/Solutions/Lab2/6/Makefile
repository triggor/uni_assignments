#Author: Igor Tryhub, 275235

CC = gcc
FLAGS = -std=gnu99 -Wall -Wextra -g -pthread

smokers: Makefile smokers.o semaphores.o
	$(CC) $(FLAGS) -o smokers smokers.o semaphores.o
	
smokers.o: smokers.c smokers.h semaphores.h
	$(CC) -c $(FLAGS) smokers.c -o smokers.o 
	
semaphores.o: semaphores.c semaphores.h
	$(CC) -c $(FLAGS) semaphores.c -o semaphores.o 
	
clean:
	rm *.o smokers
	
.PHONY: clean


	
