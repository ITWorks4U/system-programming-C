/**	system programming in C
 *
 *	How to use shared memory in C.
 *
 *	ITWorks4U
*/

#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#define	BUFFER_SIZE		256
#define	FILE_TO_USE		"shared_memory_file"
#define	PERMISSION		0644
#define	SIZE_TO_USE		65

int main(void) {
	key_t key;
	int shm_id = -1;
	char *content = NULL;

	if ((key = ftok(FILE_TO_USE, SIZE_TO_USE)) < 0) {
		perror("ftok()");
		return EXIT_FAILURE;
	}

	if ((shm_id = shmget(key, BUFFER_SIZE, PERMISSION)) < 0) {									/*	define an IPC shared memory	*/
		perror("shmget()");
		return EXIT_FAILURE;
	}

	if ((content = (char *) shmat(shm_id, NULL, 0)) == NULL) {									/*	attaching shared memory seqment	*/
		perror("shmat()");
		return EXIT_FAILURE;
	}

	printf("receiving content: %s\n", content);

	if (shmctl(shm_id, IPC_RMID, NULL) < 0) {													/*	removing shared memory			*/
		perror("shmctl()");
		return EXIT_FAILURE;
	}

	content = NULL;
	return EXIT_SUCCESS;
}
