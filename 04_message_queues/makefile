all:							message_queue_reader.o message_queue_writer.o
								gcc -g3 -Wall -o message_queue_reader.run message_queue_reader.o
								gcc -g3 -Wall -o message_queue_writer.run message_queue_writer.o
								rm *.o

message_queue_reader.o:			message_queue_reader.c
								gcc -g3 -Wall -c message_queue_reader.c

message_queue_writer.o:			message_queue_writer.c
								gcc -g3 -Wall -c message_queue_writer.c

clean:
								rm *.run
