#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "pmpi.h"

#define die(e) do {\
	fprintf(stderr, "%s error -- > %s\n", e, strerror(errno));\
	exit(EXIT_FAILURE); } while(0);

int
main(void)
{
	int status; 
	char message[BUFSIZ];
	pid_t pmpi_comm_pid;

	if (pipe(pmpi_comm_pd) == -1)
		die("pipe");

	if ((pmpi_comm_pid = fork()) == -1)
		die("fork");

	if (!pmpi_comm_pid) {
		pmpi_recv_source();
	}
	else{
		while(1){
			close(pmpi_comm_pd[0]);
			read(pmpi_comm_pd[1], message, BUFSIZ);
			printf("%s\n", message);
		}
		wait(&status);
		/* check pipe */
	}
	return 0;
	//wait(&status);
}
