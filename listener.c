#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 			5000
#define FILENAME 	"foo.c"
int
main(void) 
{
	int orig_sock;
	int new_sock;
	int	clnt_len;

	char buffer[BUFSIZ];
	int file_size;
	FILE *received_file;
	int remain_data = 0;
	struct sockaddr_in
		clnt_adr,
		serv_adr;

	int len, i;

	if((orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset (&serv_adr, 0, sizeof(serv_adr));

	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(PORT);

	if (bind(orig_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0) {
		fprintf(stderr, "Error on bind --> %s\n", strerror(errno));
		close(orig_sock);
		exit(EXIT_FAILURE);
	}

	if (listen(orig_sock, 5) < 0) {
		fprintf(stderr, "Error on listen --> %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	do {
		clnt_len = sizeof(clnt_adr);

		if ((new_sock = accept(orig_sock, (struct sockaddr *) &clnt_adr,
						&clnt_len)) < 0) {
			fprintf(stderr, "Error on accept --> %s\n", strerror(errno));
			close(orig_sock);
			exit(EXIT_FAILURE);
		}

		if (fork() == 0) {
			recv(new_sock, buffer, BUFSIZ, 0);
			file_size = atoi(buffer);

			/* attempt to open file */
			received_file = fopen(FILENAME, "w");
			if (received_file == NULL){
				fprintf(stderr, "Failed to open file --> %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			/* read from socket until we get all the expected bytes */
			remain_data = file_size;
			while (((len = recv(new_sock, buffer, BUFSIZ, 0)) > 0) 
					&&
					(remain_data > 0)){
				fwrite(buffer, sizeof(char), len, received_file);
				remain_data -= len;
			}
			fclose(received_file);
			close(new_sock);
			exit(0);
		}
		else {
			close(new_sock);
		}
	} while (1);
}
