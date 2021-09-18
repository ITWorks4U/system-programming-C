#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#include <netinet/in.h>

#define	LENGTH	127

static const unsigned int port_number = 5500;

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <CLIENT_ID>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int client_fd = -1, val_read = -1;
	struct sockaddr_in serv_addr;
	char buffer[LENGTH];
	char basic_content[LENGTH];

	memset(buffer, 0, sizeof(buffer));
	memset(basic_content, 0, sizeof(basic_content));

	strcpy(basic_content, "client with client-ID \"");
	strncat(basic_content, argv[1], strlen(argv[1]));

	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket()");
		return EXIT_FAILURE;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_number);

	/*
	*	int connect(sockfd, const struct sockaddr *addr, socklen_t addrlen);
	*
	*	sockfd		:=	client socket file descriptor
	*	addr		:=	address to use
	*	addrlen		:=	size of address
	*/
	if (connect(client_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect()");
		return EXIT_FAILURE;
	}

	printf("enter something: ");
	fgets(buffer, LENGTH, stdin);
	int buffer_length = strlen(buffer);
	buffer[buffer_length - 1] = '\0';

	strcat(basic_content, "\" wrote: ");
	strncat(basic_content, buffer, strlen(buffer));

	if (send(client_fd, basic_content, strlen(basic_content), 0) < 0) {
		perror("send()");
		return EXIT_FAILURE;
	}

	puts("message send");
	bzero(buffer, LENGTH);

	if ((val_read = read(client_fd, buffer, LENGTH)) < 0) {
		perror("read()");
		return EXIT_FAILURE;
	}

	puts(buffer);

	if (close(client_fd) < 0) {
		perror("close()");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}