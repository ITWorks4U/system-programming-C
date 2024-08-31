/*
	For UNIX/Linux this way is what you want.
	Every user has a user-ID, whereas the user root has the user-ID 0.
	By running this application as user root (with sudo) or by running
	this application as logged in user root, your user-ID will be 0,
	which allows you to do whatever you want to.

	ITWorks4U
*/

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main(void) {
	int exit_state = EXIT_FAILURE;

	/*
		you can get your user-ID by using getuid() or geteuid()

		from "man getuid"
			[...]
			getuid() returns the real user ID of the calling process.
			geteuid() returns the effective user ID of the calling process.
			[...]
	*/
	printf("user user-ID: %u\n", getuid());

	if (getuid() == 0) {
		puts("Yeah, you might be a trusted user...");
		exit_state = EXIT_SUCCESS;
	} else {
		puts("Erm... nope!");
	}

	return exit_state;
}