all: main

sorted-list.o: 
	gcc -c sorted-list.c
	
libsl.a: sorted-list.o
	ar rv libsl.a sorted-list.o

main: sorted-list.o libsl.a
	gcc -o sl main.c -L -lsl
	
clean:
	rm libsl.a sl sorted-list.o