/*
	A primitive way to check, if you have higher access rights.
	Don't, honestly, don't write and use an application like this one.
	I already did it. :o)

	ITWorks4U
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define	USER_ROOT		"I am root!"

int main(int argc, char **argv) {
	/*	defaults to a failure	*/
	int exit_state = EXIT_FAILURE;

	/*	make sure to run your application with exactly 2 arguments	*/
	if (argc == 2) {
		/*
			comparing the next argument with defined constant
			and ignore case sensitive => much worse than case sensitive
			string comparing

			by entering an expression like:
			"i am root", "I aM rOoT", "i am ", ... this works well here => in your case it might work "well", too,
			thus you don't need to write the full expression and that's NOT what you wanted!
		*/
		if (strncasecmp(USER_ROOT, argv[1], strlen(argv[1])) == 0) {
			puts("Yeah, you might have higher access rights to run this application...");
			exit_state = EXIT_SUCCESS;
		} else {
			puts("Nice try.");
		}
	} else {
		fprintf(stderr, "usage: %s <identification expression>\n", argv[0]);
	}

	return exit_state;
}