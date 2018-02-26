#Author: Igor Tryhub, 275235

CC = gcc
FLAGS = -std=gnu99 -Wall -Wextra

streams: Makefile streams.o
	$(CC) $(FLAGS) -o streams streams.o
	
streams.o: streams.c
	$(CC) -c $(FLAGS) streams.c -o streams.o 
	
clean:
	rm *.o streams
	
.PHONY: clean


	
