/**	system programming in C
 *
 *	Scanning the current directory and list all files with their attributes.
 *	Attention:
 *		By using this sample you can scan your current directory only.
 *		This is caused by the stat function for each file, which is shall exist
 *		on the current working directory of your launched application. So don't be
 *		confused, if a file might not exist, which really exist.
 *
 *		To fix that logical error, it's required to use the absolute path.
 *		Take a look to the advanced folder.
 *
 *	ITWorks4U
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

/*	system libraries to use	*/
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

/*	constants	*/
#define	RWX			"rwxrwxrwx"
#define	MAX_LENGTH	9
#define	INFO		"INFO:"
#define	WARNING		"WARNING:"
#define	ERROR		"ERROR:"

/// @brief Scanning the current folder path.
/// @param path given path to scan
void scan_folder(const char *path) {
	bool on_continue = true;

	DIR *current_dir = NULL;

	/*
		DIR *opendir(const char *name);

		Opening the current directory.
		On success, a pointer for DIR returns,
		otherwise NULL returns and errno is set.
	*/

	if ((current_dir = opendir(path)) == NULL) {
		fprintf(stderr, "%s current directory \"%s\" was unable to open: %s skipping...\n", ERROR, path, strerror(errno));
		on_continue = false;
	}

	if (on_continue) {
		/*	bit flags for any file	*/
		int bits[]= {
			S_IRUSR,S_IWUSR,S_IXUSR,	/*	current user	*/
			S_IRGRP,S_IWGRP,S_IXGRP,	/*	current group	*/
			S_IROTH,S_IWOTH,S_IXOTH		/*	other			*/
		};

		struct dirent *dir_ptr = NULL;

		/*
			struct dirent *readdir(DIR *__dirp);

			Read the next detected file. "File" can be a regular file, a folder, device, link, ...
			When the end has been reached, NULL returns.
		*/

		while ((dir_ptr = readdir(current_dir)) != NULL) {
			struct stat s;

			if (stat(dir_ptr->d_name, &s) < 0) {
				/*	in case of an error the current file is going to skip	*/
				fprintf(stdout, "%s unable to receive file stat for \"%s\": %s skipping...\n", WARNING, dir_ptr->d_name, strerror(errno));
				continue;
			}

			/*
				struct stat contains properties for the file;
				depending on your working system, C version, ...
				the amount of properties may differ

				There're macro functions to determine which
				file is currently in use.
			*/
			if (S_ISREG(s.st_mode)) {
				printf("regular file...\t\t");
			} else if (S_ISDIR(s.st_mode)) {
				printf("folder...\t\t");
			} else if (S_ISCHR(s.st_mode)) {
				printf("character device...\t\t");
			} else if (S_ISBLK(s.st_mode)) {
				printf("block device...\t\t");
			} else if (S_ISFIFO(s.st_mode)) {
				printf("FIFO/pipe...\t\t");
			} else if (S_ISLNK(s.st_mode)) {
				printf("linked file...\t\t");
			} else {
				printf("unknown...\t\t");
			}

			char file_access[MAX_LENGTH];
			memset(file_access, '\0', MAX_LENGTH);
			printf("%-20s [", dir_ptr->d_name);

			for(int j = 0; j < MAX_LENGTH; j++) {
				file_access[j] = (s.st_mode & bits[j]) ? RWX[j] : '-';
			}
			printf("%s]\n", file_access);
		}

		/*
			Closes the directory stream.

			int closedir(DIR *__dirp);

			on success, 0 returns
			on failure, -1 returns and errno is set
		*/
		if (closedir(current_dir) < 0) {
			perror("closedir()");
			exit(EXIT_FAILURE);
		}
	}
}

/// @brief Entry point -> 1 argument is required
/// @param argc argument counter
/// @param argv argument vector
/// @return EXIT_SUCCESS / EXIT_FAILURE
int main(int argc, char **argv) {
	/*
		Don't be confused about "argc != 2".
		The first argument is your application itself.
	*/

	if (argc != 2) {
		fprintf(stdout, "%s usage: %s <path>\n", INFO, argv[0]);
		return EXIT_FAILURE;
	}

	scan_folder(argv[1]);
	return EXIT_SUCCESS;
}