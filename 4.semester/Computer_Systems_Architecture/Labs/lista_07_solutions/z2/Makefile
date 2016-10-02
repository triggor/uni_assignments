zad2 : Makefile   lcm_gcd.o test.o 
	gcc -o zad2  lcm_gcd.o test.o 

test.o : Makefile   test.c
	gcc -c test.c -o  test.o

lcm_gcd.o : Makefile   lcm_gcd.s 
	as lcm_gcd.s -o lcm_gcd.o  
