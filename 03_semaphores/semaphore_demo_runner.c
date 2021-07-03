/**	system programming in C
 *
 *	How to use sempahores in C.
 *
 *	ITWorks4U
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "semaphore_demo.h"

#define	P_SHARED_OPTION				0
#define	INITIAL_VALUE_SEMAPHORE		1

int main(void) {
	if (sem_init(&mutex_to_use, P_SHARED_OPTION, INITIAL_VALUE_SEMAPHORE) < 0) {
		perror("sem_init()");
		return EXIT_FAILURE;
	}

	pthread_t thread_01, thread_02;

	if (pthread_create(&thread_01, NULL, thread_function, NULL) < 0) {
		perror("pthread_create(1)");
		return EXIT_FAILURE;
	}

	sleep(2);

	if (pthread_create(&thread_02, NULL, thread_function, NULL) < 0) {
		perror("pthread_create(2)");
		return EXIT_FAILURE;
	}

	if (pthread_join(thread_01, NULL) < 0) {
		perror("pthread_join()");
		return EXIT_FAILURE;
	}

	if (pthread_join(thread_02, NULL) < 0) {
		perror("pthread_join()");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}