/*
	For Windows this way might work for you, too.
	Before you're compiling make sure, your WINNT has a
	value of 0x600 (in decimal: 1,536) or higher.
	Or in other words: You OS must be a Windows Server 2008 or higher.

	The WINNT version tells you, which OS you're working on.
	Even on a Windows 10 (tested here) it tells you, that you're
	running a "Windows 2000", which is wrong.

	For more informations take a look here:
	-	https://learn.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=msvc-170

	ITWorks4U
*/

/*	override your WIN32_WINNT, if necessary	*/
#define _WIN32_WINNT	0x600

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

/*
	remember:
		this has been built in C
		in C++ it might have a different behavior
*/
bool on_admin(void) {
	bool onAdmin = false;
	bool onContinue = true;
	HANDLE hToken = NULL;
	TOKEN_ELEVATION elevation;
	DWORD size;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		fprintf(stderr, "failed to get process token: %lu\n", GetLastError());
		onContinue = false;
	}

	if (onContinue && !GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size)) {
		fprintf(stderr, "failed to get token information: %lu\n", GetLastError());
		onContinue = false;
	}

	if (onContinue) {
		onAdmin = elevation.TokenIsElevated;
		if (hToken != NULL) {
			CloseHandle(hToken);
			hToken = NULL;
		}
	}
	
	return onAdmin;
}

int main(void) {
	if (on_admin()) {
		puts("Yeah, you might be a user with admin rights...");
	} else {
		puts("Erm... nope!");
	}

	return EXIT_SUCCESS;
}