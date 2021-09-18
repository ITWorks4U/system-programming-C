all:			server.o client.o
				gcc -g3 -Wall -o server.run server.o
				gcc -g3 -Wall -o client.run client.o
				rm *.o

server.o:		server.c
				gcc -c server.c

client.o:		client.c
				gcc -c client.c

clean:
				rm *.run