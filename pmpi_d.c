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
int pmpi_comm_pd[2];
int pmpi_msg_pd[2];

int
create_pipe(void)
{
	return pipe(pmpi_comm_pd) | pipe(pmpi_msg_pd);
}

int
main(void)
{
	int status; 
	char message[256];
	int bytes = 0;
	pid_t pmpi_comm_pid;
	if (create_pipe() == -1)
		die("pipe");

	if ((pmpi_comm_pid = fork()) == -1)
		die("fork");

	if (!pmpi_comm_pid) {
		close(pmpi_comm_pd[0]);
		pmpi_recv_source();
	}
	else{
		//if (!fork()){
			//pmpi_recv_msg_direct();
		//}
		//else{
			while(1){
				memset(&message, 0, sizeof(message));
				bytes = read(pmpi_comm_pd[0], message, 256);
				//printf("%s\n", strerror(errno));
				//printf("%d\n", bytes);
				if (bytes > 0){
					//printf("%d\n", bytes);
					//printf("%s\n", message);
					//printf("%d\n", strlen(message));
					pmpi_compile(message);
					pmpi_run();
				}
			}
			wait(&status);
		//}
	}
	return 0;
	wait(&status);
}
