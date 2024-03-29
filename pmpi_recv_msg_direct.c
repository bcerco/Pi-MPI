#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include "pmpi.h"

#define PORT 6662

void
pmpi_recv_msg_direct(void)
{
	int orig_sock;
	int new_sock;
	int fd;
	socklen_t	clnt_len;
	int status;
	int rank = 0;
	char buffer[BUFSIZ];
	char path[256];
	//printf("BUFSIZ: %d\n", BUFSIZ);
	struct sockaddr_in
		clnt_adr,
		serv_adr;

	int len;

	/* create orig_socket */
	if((orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset (&serv_adr, 0, sizeof(serv_adr));

	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(PORT);

	/* attempt to bind orig_sock */
	if (bind(orig_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0) {
		fprintf(stderr, "Error on bind --> %s\n", strerror(errno));
		close(orig_sock);
		exit(EXIT_FAILURE);
	}

	/* listen on orig_sock */
	if (listen(orig_sock, 5) < 0) {
		fprintf(stderr, "Error on listen --> %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* loop indefinitely */
	do {
		clnt_len = sizeof(clnt_adr);
		/* attempt to accept the socket */
		if ((new_sock = accept(orig_sock, (struct sockaddr *) &clnt_adr,
						&clnt_len)) < 0) {
			fprintf(stderr, "Error on accept --> %s\n", strerror(errno));
			close(orig_sock);
			exit(EXIT_FAILURE);
		}

		if (fork() ==  0) {
			/* get the rank of the sender */
			recv(new_sock, &rank, sizeof(rank), 0);
			/* open the corresponding FIFO */
			sprintf(path, "node%d.fifo", rank);
			/* ACK for the rank message */
			send(new_sock, &rank, sizeof(rank), 0);
			/* open the FIFO write only */
			fd = open(path, O_WRONLY);
			//printf("%s\n", strerror(errno));
			//printf("%d\n", fd);
			while (((len = recv(new_sock, buffer, BUFSIZ, 0)) > 0)){
				//printf("%s\n", buffer);
				write(fd, buffer, len);
				memset(&buffer, 0, sizeof(buffer));
			}
			//printf("PID: %d\n", getpid());
			close(fd);
			close(new_sock);
			_exit(0);
		}
		else {
			//wait(&status);
			close(new_sock);
		}
	} while (1);
}
