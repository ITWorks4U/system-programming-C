/**	system programming in C
 *
 *	Scanning the current directory including sub directories and list all files with their attributes.
 *
 *	In contrast to the basic version, this works also for any given path.
 *	contains:
 *	-	scanning the given path
 *	-	ignoring "." and ".."
 *	-	in case of a directory has been found, scan also this directory
 *	-	check, if the current file is a softlink
 *	-	display the size of the file in a human readable format
 *
 *	ITWorks4U
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

/*	constants	*/
#define	SEPARATOR	"--------------"
#define	CURR_DIR	"."
#define	SUPER_DIR	".."
#define	SLASH		"/"
#define	MAX_PATH_LENGTH	255
#define	RWX		"rwxrwxrwx"
#define	BUFFER_SIZE	9
#define	KILOBYTE	1024L
#define	MEGABYTE	KILOBYTE*1024L
#define	GIGABYTE	MEGABYTE*1024L

/*	global variables	*/
static char full_path[MAX_PATH_LENGTH];		/*	absolute path	*/
static char link_path[MAX_PATH_LENGTH];		/*	link path for a softlink	*/
static int bits[]= {
	S_IRUSR,S_IWUSR,S_IXUSR,
	S_IRGRP,S_IWGRP,S_IXGRP,
	S_IROTH,S_IWOTH,S_IXOTH
};

/// @brief Creating the absolute path. It's required to be able to detect any data in a foreign folder of the current working directory.
/// @param path path to use
/// @param data file to append
void create_full_path(const char *path, const char *data) {
	memset(full_path, '\0', MAX_PATH_LENGTH);
	strncpy(full_path, path, strlen(path));

	/*	warning: ‘strncat’ specified bound 1 equals source length [-Wstringop-overflow=]	*/
	//strncat(full_path, SLASH, strlen(SLASH));

	/*	unsafe, but the warning above won't appear	*/
	strcat(full_path, SLASH);

	strncat(full_path, data, strlen(data));

	/*
		result:	/<the absolute path>/data
	
		The absolute path can also be determined by realpath() from stdlib,
		but it may have a different result. Make sure this function is
		defined on your working machine.
	*/
}

/// @brief check, if the current path refers to a soft linked path
/// @param path path to check
/// @return true, if a softlink has been detected, false otherwise
bool on_softlink(const char *path) {
	memset(link_path, '\0', MAX_PATH_LENGTH);

	/*
		Read the link to the given pathname and copy the result to buf with the max length of bufsiz.

		ssize_t readlink(const char *restrict pathname, char *restrict buf, size_t bufsiz);

		returns:
			-	>= 0 on success
			-	< 0 on failure (by default -1 only)
	*/
	ssize_t on_softlink = readlink(path, link_path, MAX_PATH_LENGTH);

	if (on_softlink >= 0) {
		link_path[on_softlink] = '\0';
		return true;
	}

	return false;
}

/// @brief Checks, if the current directory path is "." or ".."
/// @param path path to check
/// @return true, if "." or ".." has been detected, false otherwise
bool on_current_or_super_directory(const char *path) {
	return	strncmp(path, CURR_DIR, strlen(CURR_DIR)) == 0 ||
			strncmp(path, SUPER_DIR, strlen(SUPER_DIR)) == 0;
}

/// @brief Checks, if the current path contains a directory.
/// @param path path to check
/// @return true, if a directory has been found, false otherwise
bool on_directory(const char *path) {
	struct stat s;
	if (stat(path, &s) < 0) {
		/*	in case of any error	*/
		fprintf(stderr, "error for \"%s\": %s\n", path, strerror(errno));
		return false;
	}

	/*
		Depending on the current working machine, bit technology, C version, ...
		the macros may be different. On older systems, the macros S_IFMT and
		S_IFDIR were in use.
	*/
	return (s.st_mode & __S_IFMT) == __S_IFDIR;
}

/// @brief Determine the type of a file
/// @param s certain file mode
/// @return the certain file type
char determine_type(const struct stat *s) {
	if (S_ISREG(s->st_mode)) {
		return '-';
	} else if (S_ISDIR(s->st_mode)) {
		return 'd';
	} else if (S_ISCHR(s->st_mode)) {
		return 'c';
	} else if (S_ISBLK(s->st_mode)) {
		return 'b';
	} else if (S_ISFIFO(s->st_mode)) {
		return 'p';
	} else if (S_ISLNK(s->st_mode)) {
		return 'l';
	}/* else if (S_ISSOCK(s->st_mode)) {
		return 's';

		//	On >>this<< current working machine (Linux Mint 21.1 64 bit) S_ISSOCK was not available.
	}*/

	/*
		Depending on the current working machine, bit technology, C version, ...
		there might also be other macro functions (un-)available.
	*/

	return '?';
}

/// @brief Human readable representation for a file size.
/// @param size the size of a file on bytes only
void human_readable_size(long size) {
	if (size >= KILOBYTE && size < MEGABYTE) {
		printf("%ld KB", size / KILOBYTE);
	} else if (size >= MEGABYTE && size < GIGABYTE) {
		printf("%ld MB", size / MEGABYTE);
	} else if (size >= GIGABYTE) {
		printf("%ld GB", size / GIGABYTE);
	} else {
		if (size == 1) {
			printf("%ld B", size);
		} else {
			printf("%ld B", size);
		}
	}

	printf("\t");
}

/// @brief Scanning the current folder path.
/// @param name given path to scan
void display_folder_content(const char *name) {
	puts(SEPARATOR);

	/*	get the absolute path (from stdlib)	*/
	char *ptr = realpath(name, NULL);
	printf("scanning path: \"%s\"\n", name);
	puts("mode\t\t\t\tname\tUID\tGID\tsize\tlast access\tlast modification\tis softlink");

	DIR *current = NULL;
	if ((current = opendir(name)) == NULL) {
		perror("opendir()");
		exit(EXIT_FAILURE);
	}

	struct dirent *dirptr = NULL;
	while((dirptr = readdir(current)) != NULL) {
		/*	ignoring "." and ".."	=>	could have side effects for the code below	*/
		if (on_current_or_super_directory(dirptr->d_name)) {
			continue;
		}

		create_full_path(ptr, dirptr->d_name);
		struct stat file_stat;
		char file_access[BUFFER_SIZE];
		memset(file_access, '\0', BUFFER_SIZE);

		/*	with the known full path stat shall not return -1	*/
		if (stat(full_path, &file_stat) < 0) {
			fprintf(stdout, "unable to receive data for \"%s\": %s skipping...\n", dirptr->d_name, strerror(errno));
			continue;
		}

		/*	listing data properties for certain file; similar to "ls" command	*/
		for(int j = 0; j < BUFFER_SIZE; j++) {
			file_access[j] = (file_stat.st_mode & bits[j]) ? RWX[j] : '-';
		}

		/*
			output:
			<file type> <file bit modes> <name> <uid> <gid> <human readable file size> <last access time> <last modification time> <softlink?>

			attention:
				<last access time> <last modification time> contains the absolute elapsed time from January 1st 1970 in seconds
				These shall be converted into a datetime format. => By default there's no known C-function, which can do this.
		*/
		printf("%c%s\t%20s\t%d\t%d\t", determine_type(&file_stat), file_access, dirptr->d_name, file_stat.st_uid, file_stat.st_gid);
		human_readable_size(file_stat.st_size);
		printf("%ld\t%ld\t\t", file_stat.st_atime, file_stat.st_mtime);

		bool softlink = on_softlink(full_path);
		printf("%s", softlink ? "yes" : "no");
		if (softlink) {
			printf(" -> %s\n", link_path);
		} else {
			puts("");
		}

		/*	whenever a directory has been detected, scan this directory, too	*/
		if (on_directory(full_path)) {
			display_folder_content(full_path);
		}
	}

	closedir(current);
	puts(SEPARATOR);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage %s <path to look for>\n", argv[0]);
		return EXIT_FAILURE;
	}

	display_folder_content(argv[1]);
	return EXIT_SUCCESS;
}