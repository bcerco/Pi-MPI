#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include "pmpi.h"

#define PORT 			6665

void
pmpi_recv_source(void) 
{
	int orig_sock;
	int new_sock;
	socklen_t	clnt_len;
	int status;
	char message[256];
	char buffer[BUFSIZ];
	int ack = 1;
	printf("BUFSIZ: %d\n", BUFSIZ);
	int file_size;
	FILE *received_file;
	int remain_data = 0;
	char *file_name;
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
			/* get the size in bytes of the incoming file */
			recv(new_sock, buffer, BUFSIZ, 0);
			file_size = atoi(buffer);
			/* send ack */
			send(new_sock, &ack, sizeof(ack), 0);
			memset(&buffer, 0, BUFSIZ);
			/* get file name */
			recv(new_sock, buffer, BUFSIZ, 0);
			/* send ack */
			file_name = strdup(buffer);
			send(new_sock, &ack, sizeof(ack), 0);
			memset(&buffer, 0, BUFSIZ);
			/* attempt to open file */
			received_file = fopen(file_name, "w");
			if (received_file == NULL){
				fprintf(stderr, "Failed to open file --> %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			/* read from socket until we get all the expected bytes */
			remain_data = file_size;
			while (((len = recv(new_sock, buffer, BUFSIZ, 0)) > 0) 
					&&
					(remain_data > 0)){
				printf("Remain: %d, Len: %d\n", remain_data, len); 
				fwrite(buffer, sizeof(char), len, received_file);
				remain_data -= len;
			}
			printf("%s\n", strerror(errno));
			fclose(received_file);
			/* write file_name to pipe */
			write(pmpi_comm_pd[1], file_name, strlen(file_name));
			free(file_name);
			close(new_sock);
			_exit(0);
		}
		else {
			wait(&status);
			close(new_sock);
		}
	} while (1);
}
