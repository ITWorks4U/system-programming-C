/**	system programming in C
 *
 *	How to use message queues in C.
 *
 *	ITWorks4U
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ipc.h>																/*	these libraries are required for message queue	*/
#include <sys/msg.h>
#include <sys/types.h>

#define	PERMISSION_OPTION		0644												/*	rw-r--r-- for the file	*/
#define	BUFFER_SIZE				200
#define	FILE_TO_USE				"message_queue_holder"								/*	make sure this file exists on your current system	*/
#define	SIZE_TO_USE				65

struct MessageQueue {
	char m_text[BUFFER_SIZE];
};

int main(void) {
	struct MessageQueue mq;
	int msg_id, str_length;
	key_t key_to_use;

	if ((key_to_use = ftok(FILE_TO_USE, SIZE_TO_USE)) < 0) {						/*	uses the identity of the given pathname and the least significant 8 bits of proj_id	*/
		perror("ftok()");
		return EXIT_FAILURE;
	}

	if ((msg_id = msgget(key_to_use, PERMISSION_OPTION | IPC_CREAT)) < 0) {			/*	get a System V message queue identifier	*/
		perror("msgget()");
		return EXIT_FAILURE;
	}

	printf("enter a text: ");
	fgets(mq.m_text, sizeof(mq.m_text), stdin);
	str_length = strlen(mq.m_text);
	mq.m_text[str_length - 1] = '\0';												/*	replacing \n to \0	*/

	if (msgsnd(msg_id, &mq, str_length, 0) < 0) {									/*	send a message trough the queue */
		perror("msgsnd()");
		return EXIT_FAILURE;
	}

	if (msgctl(msg_id, IPC_RMID, NULL) < 0) {
		perror("msgctl()");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}