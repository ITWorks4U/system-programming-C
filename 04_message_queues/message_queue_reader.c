/**	system programming in C
 *
 *	How to use message queues in C.
 *
 *	ITWorks4U
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define	PERMISSION_OPTION		0644												/*	rw-r--r-- for the file	*/
#define	BUFFER_SIZE				200
#define	FILE_TO_USE				"message_queue_holder"
#define	SIZE_TO_USE				65

struct MessageQueue {
	char m_text[BUFFER_SIZE];
};

int main(void) {
	struct MessageQueue mq;
	int msg_id = -1;
	key_t key;

	if ((key = ftok(FILE_TO_USE, SIZE_TO_USE)) < 0) {
		perror("ftok()");
		return EXIT_FAILURE;
	}

	if ((msg_id = msgget(key, PERMISSION_OPTION)) < 0) {
		perror("msgget()");
		return EXIT_FAILURE;
	}

	if (msgrcv(msg_id, &mq, sizeof(mq.m_text), 0, 0) < 0) {							/*	receiving the content of the message queue	*/
		perror("msgrcv()");
		return EXIT_FAILURE;
	}

	printf("%s\n", mq.m_text);

	return EXIT_SUCCESS;
}