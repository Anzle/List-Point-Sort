all: main

sorted-list.o: 
	gcc -Wall -g -c sorted-list.c
	
libsl.a: sorted-list.o
	ar rv libsl.a sorted-list.o

main: main.c libsl.a
	gcc -Wall -g -o sl main.c libsl.a -lm 
	
clean:
	rm libsl.a sl sorted-list.o