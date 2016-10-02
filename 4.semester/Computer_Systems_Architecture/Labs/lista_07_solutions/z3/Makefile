zad3 : Makefile   insert_sort.o test.o 
	gcc -o zad3  insert_sort.o test.o 

test.o : Makefile   tets.c
	gcc -c tets.c -o  test.o

insert_sort.o : Makefile   insert_sort.s 
	as insert_sort.s -o insert_sort.o 