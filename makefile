all: main_prova.o bmp.o ip_lib.o
	gcc main_prova.o bmp.o ip_lib.o -o prova -lm -Wall --ansi --pedantic

main_prova.o: main_prova.c
	gcc -c main_prova.c -Wall

ip_lib.o: ip_lib.c ip_lib.h
	gcc -c ip_lib.c -Wall

bmp.o: bmp.c
	gcc -c bmp.c -Wall

clean:
	rm -f *.o prova