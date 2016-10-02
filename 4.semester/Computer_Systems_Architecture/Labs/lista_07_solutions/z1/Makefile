CPP = gcc

zad1 : Makefile   clz.o test.o 
	gcc -o zad1  clz.o test.o 

test.o : Makefile   test.c
	gcc -c test.c -o  test.o

clz.o : Makefile   clz.s 
	as clz.s -o clz.o 
	
