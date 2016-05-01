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

void
pmpi_send_msg(void *buf, int type, int size, int rank)
{
	FILE *f_host;
	size_t len = 0;
	ssize_t read;
	char *dest = NULL;
	int count = 0;
	/* open the file that contains the addresses of all nodes */
	f_host = fopen("hosts.txt", "r");
	if (f_host == NULL){
		fprintf(stderr, "Error opening hosts.txt --> %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* get the address of the supplied rank */
	while ((read = getline(&dest, &len, f_host)) != -1){
		if (count == rank){
			dest[strlen(dest) -1] = '\0';
			break;
		}
		count++;
	}
	switch(type){
		case 0:
			pmpi_send_msg_direct(dest, (int *) buf, size, rank);
			break;
		case 1:
			pmpi_send_msg_direct(dest, (char *) buf, size, rank);
			break;
		case 2:
			pmpi_send_msg_direct(dest, (float *) buf, size, rank);
			break;
		default:
			break;
	}
}
