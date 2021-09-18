#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define	LENGTH	127

static const unsigned int port_number = 5500;

int main(void) {
	int server_fd = -1, new_socket = -1, val_read = -1;
	struct sockaddr_in address;
	int addr_length = sizeof(address);
	char buffer[LENGTH];
	memset(buffer, 0, sizeof(buffer));

	/*
	*	int socket(domain, type, protocol);
	*
	*	domain		:=	communication domain, like AF_INET (IPv4), AF_INET6 (IPv6)
	*	type		:=	communication type, where SOCK_STREAM = TCP; SOCK_DGRAM = UDP
	*	protocol	:=	protocol value for IP; optional flag, by default protocol is 0	->	man protocols
	*
	*/
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {																		/*	creating socket file descriptor	*/
		perror("socket()");
		return EXIT_FAILURE;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port_number);

	/*
	*	int bind(sockfd, const struct sockaddr *addr, socklen_t addrlen);
	*
	*	sockfd		:=	socket file descriptor
	*	addr		:=	address struct to use, where a cast is required
	*	addrlen		:=	size of the used structure
	*/
	if (bind(server_fd, (struct sockaddr *) &address, addr_length) < 0) {															/*	binding address	*/
		perror("bind()");
		return EXIT_FAILURE;
	}

	const int max_nbr_clients = 3;
	
	/*
	*	int listen(sockfd, backlog);
	*
	*	sockfd		:=	socket file descriptor
	*	backlog		:=	max. number of clients to listen
	*/
	if (listen(server_fd, max_nbr_clients) < 0) {																					/*	offer a queue for up to three clients	*/
		perror("listen()");
		return EXIT_FAILURE;
	}

	puts(" quit with \"CTRL+C\"");
	while (true) {

		/*
		*	int accept(sockfd, struct sockaddr *addr, socklen_t *addrlen);
		*
		*	sockfd	:=	socket file descriptor
		*	addr	:=	source address
		*	addrlen	:=	size of source address
		*/
		if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t  *) &addr_length)) < 0) {						/*	accepting client request	*/
			perror("accept()");
			return EXIT_FAILURE;
		}

		/*
		*	int read(sockfd, void *buffer, size_t count);
		*
		*	sockfd	:=	socket file descriptor
		*	buffer	:=	text buffer
		*	count	:=	size of text buffer
		*/
		if ((val_read = read(new_socket, buffer, LENGTH)) < 0) {																	/*	read the content from the client	*/
			perror("read()");
			return EXIT_FAILURE;
		}

		puts(buffer);
		char answer[] = "Hello from server!";

		/*
		*	int send(sockfd, const void *buf, size_t len, int flags);
		*
		*	sockfd	:=	socket file descriptor
		*	buf		:=	text buffer
		*	len		:=	length of buf
		*	flags	:=	given flags, if required; default value: 0 -> man send
		*/
		if (send(new_socket, answer, strlen(answer), 0) < 0) {																		/*	sending welcome message to the client	*/
			perror("send()");
			return EXIT_FAILURE;
		}

		memset(buffer, 0, sizeof(buffer));
	}

	if (close(server_fd) < 0) {
		perror("close()");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}