#using semaphore_demos in C
#linking program with -lpthread and -lrt

all:						semaphore_demo_runner.o semaphore_demo.o
							gcc -g3 -Wall -o semaphore_demo_example.run semaphore_demo_runner.o semaphore_demo.o -lpthread -lrt
							rm *.o

semaphore_demo_runner.o:	semaphore_demo_runner.c semaphore_demo.h
							gcc -c -Wall -g3 semaphore_demo_runner.c

semaphore_demo.o:			semaphore_demo.c semaphore_demo.h
							gcc -c -Wall -g3 semaphore_demo.c

clean:
							rm semaphore_demo_example.run
