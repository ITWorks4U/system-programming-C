/**	system programming in C
 *
 *	How to use pipes with fork in C.
 *
 *	ITWorks4U
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define	BUFFER_LENGTH		100

int main(void) {
	int pipe_file_descriptor[2] = {0};
	int nbr_read = -1;
	int pid_nbr = -1;
	char buffer[BUFFER_LENGTH] = {'\0'};
	char text_to_use[] = "Hello from parent!";

	if (pipe(pipe_file_descriptor) < 0) {
		perror("pipe()");
		return EXIT_FAILURE;
	}

	switch(pid_nbr = fork()) {															/*	for more details: see C programming part 40	*/
		case -1: {
			perror("fork()");
			return EXIT_FAILURE;
		} case 0: {																		/*	child process		*/
			if (close(pipe_file_descriptor[1]) < 0) {									/*	closing write pipe	*/
				perror("close()");
				return EXIT_FAILURE;
			}

			nbr_read = read(pipe_file_descriptor[0], buffer, sizeof(buffer));			/*	reading from pipe	*/
			switch(nbr_read) {
				case -1:
					perror("read()");
					return EXIT_FAILURE;
				case 0:
					fprintf(stderr, "EOF / empty pipe\n");
					return EXIT_FAILURE;
				default:
					printf("read %d bytes from \"%s\"\n", nbr_read, buffer);
			}

			break;

		} default: {																	/*	parent process		*/
			if (close(pipe_file_descriptor[0]) < 0) {									/*	closing read pipe	*/
				perror("close()");
				return EXIT_FAILURE;
			}

			if (write(pipe_file_descriptor[1], text_to_use, strlen(text_to_use)) < 0) {	/*	writing content to the pipe	*/
				perror("write()");
				return EXIT_FAILURE;
			}

			if (waitpid(pid_nbr, NULL, WUNTRACED) < 0) {								/*	waiting for child	*/
				perror("waitpid()");
				return EXIT_FAILURE;
			}
		}
	}

	return EXIT_SUCCESS;
}
