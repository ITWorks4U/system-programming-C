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

void *thread_function(void *arg) {
	/*	int sem_wait(sem_t *sem);
	*	Locks/Decrements a semaphore pointed to by sem.
	*
	*	on success: 0 returns, -1 otherwise and errno is set
	*/
	if (sem_wait(&mutex_to_use) < 0) {
		perror("sem_wait()");
		return NULL;
	}

	puts("entering function...");
	int i;

	for (i = 0; i < 5; i++) {
		printf("sleeping for thread %lu...\n", pthread_self());
		sleep(1);
	}

	/*	int sem_post(sem_t *sem);
	*	Unlocks/Increments the semaphore pointed by sem.
	*
	*	on success: 0 returns, -1 otherwise and errno is set
	*/
	if (sem_post(&mutex_to_use) < 0) {
		perror("sem_post()");
		return NULL;
	}

	return NULL;
}