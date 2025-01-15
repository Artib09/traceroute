CC = gcc
F = -W -Wall -O2

traceroute: Makefile traceroute.o utils.o send.o
		$(CC) $(F) -o traceroute traceroute.o send.o utils.o

utils.o: Makefile utils.c
		$(CC) $(F) -c utils.c -o utils.o
		
send.o: Makefile send.c
		$(CC) $(F) -c send.c -o send.o

traceroute.o: Makefile traceroute.c traceroute.h
		$(CC) $(F) -c traceroute.c -o traceroute.o

clean :
	rm -rf *.o main
