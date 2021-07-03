/**	system programming in C
 *
 *	handle with files
 *
 *	ITWorks4U
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>																/*	in use for errno		*/
#include <string.h>																/*	in use for strerror()	*/
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define	FILE_NAME_TO_USE	"testfile.txt"										/*	file to use				*/

int main(void) {
	/****	USER LAYER	****/
	FILE *handler = NULL;

	if ((handler = fopen(FILE_NAME_TO_USE, "r")) == NULL) {
		perror("fopen()");														/*	prints the given text including a detailed error message	*/
		fprintf(stderr, "%s\n", strerror(errno));								/*	prints an error message depending on errno					*/
		return EXIT_FAILURE;
	}

	if (access(FILE_NAME_TO_USE, F_OK | R_OK) < 0) {
		perror("access()");
		return EXIT_FAILURE;
	}

	/*	do other stuff	*/

	if (fclose(handler) < 0) {
		perror("fclose()");
		return EXIT_FAILURE;
	}

	/*	FILE* fopen(const char *pathname, const char *mode);
	*	Opens a file to handle with it.
	*
	*	int access(const char *pathname, int mode);
	*	Check user's permissions for a file.
	*
	*	int fclose(FILE *stream);
	*	Closes a stream.
	*/

	/*	----------------------------------------------	*/
	/****	SYSTEM LAYER	****/

	int file_descriptor = -1;

	if ((file_descriptor = open(FILE_NAME_TO_USE, O_WRONLY)) < 0) {				/*	opens a file, where write operations only are planned	*/
		perror("open()");
		return EXIT_FAILURE;
	}

	char buffer[] = "This is a simple text.";
	int nbr_written;

	nbr_written = write(file_descriptor, buffer, sizeof(buffer));				/*	write content of buffer to the file				*/
	switch(nbr_written) {
		case -1:
			perror("write()");
			return EXIT_FAILURE;
		case 0:
			fprintf(stderr, "No content has been written!\n");
			return EXIT_FAILURE;
		default:
			printf("%d bytes has been written\n", nbr_written);
	}

	if (close(file_descriptor) < 0) {											/*	closes a file descritor							*/
		perror("close()");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
