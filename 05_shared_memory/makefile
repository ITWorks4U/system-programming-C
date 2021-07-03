all:							shared_memory_reader.o shared_memory_writer.o
								gcc -g3 -Wall -o shared_memory_reader.run shared_memory_reader.o
								gcc -g3 -Wall -o shared_memory_writer.run shared_memory_writer.o
								rm *.o

shared_memory_reader.o:			shared_memory_reader.c
								gcc -g3 -Wall -c shared_memory_reader.c

shared_memory_writer.o:			shared_memory_writer.c
								gcc -g3 -Wall -c shared_memory_writer.c

clean:
								rm *.run
