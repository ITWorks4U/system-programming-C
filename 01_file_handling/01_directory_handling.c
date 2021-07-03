/**	system programming in C
 *
 *	handle with directories
 *
 *	ITWorks4U
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>

enum {
	OTHER_FILE = 0,
	DIRECTORY = 1
};

int isDirectory(const char *fileName) {
	struct stat *file_stat = calloc(1, sizeof(struct stat));
	assert(file_stat != NULL);

	/*	int stat(const char *path, struct stat *buf);
	*	lists properties of a file given by path and stores it to buf
	*/
	stat(fileName, file_stat);
	printf(" size: %ld bytes ", file_stat->st_size);					/*	prints the size of the file							*/

	mode_t file_type = file_stat->st_mode & S_IFMT;						/*	get the file type, where 'd' represents a directory	*/
	if (file_type == S_IFDIR) {											/*	Have we found a directory?							*/
		free(file_stat);
		return DIRECTORY;												/*	Then return DIRECTORY as a custom indicator			*/
	}

	free(file_stat);
	return OTHER_FILE;													/*	otherwise any other file found						*/
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage %s <file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	/*	DIR *opendir(const char *name);
	*	Opens a directory stream corresponding to the directory name. On success,
	*	it returns a function pointer to the directory stream, on fail, NULL returns
	*	and errno is set.
	*/

	DIR *dir_ptr = NULL;
	if ((dir_ptr = opendir(argv[1])) == NULL) {
		perror("opendir()");
		return EXIT_FAILURE;
	}

	/*	struct dirent *readdir(DIR *dirp);
	*	Reads the content of directory dirp, where the first found file
	*	is read only, unless we're using a loop.
	*/
	struct dirent *dir_content = NULL;
	while ((dir_content = readdir(dir_ptr))) {
		printf("file: %s -> ", dir_content->d_name);
		int file_result = isDirectory(dir_content->d_name);

		switch(file_result) {
			case OTHER_FILE:
				fprintf(stdout, "is not a directory\n"); 
				break;
			case DIRECTORY:
				fprintf(stdout, "is a directory\n"); 
				break;
		}
	}

	if (closedir(dir_ptr) < 0) {											/*	closes a DIR* stream	*/
		perror("closedir()");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
