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
main(int argc, char ** argv)
{
	int status;
	int out = 0;
	int pd[2];
	char *source_code;
	char *binary_file;
	char buffer[BUFSIZ];
	memset(&buffer, 0, BUFSIZ);
	pid_t pid;

	/* create pipe to redirect child process stderr */
	if (pipe(pd) == -1)
		die("pipe");
	/* exit if too few arguments */
	if (argc < 3){
		fprintf(stderr, "Usage: %d\n", argc);
		exit(EXIT_FAILURE); 
	}
	source_code = strdup(argv[1]);
	binary_file = strtok(argv[1], ".");
	//char *const compile_params[] = {"gcc", source_code, "-o", binary_file, NULL};
	char *const compile_params[] = {"gcc", source_code, "pmpi_init.c","-o", "prog", NULL};

	/* fork and exec to compile program */
	if ((pid = fork()) == -1)
		die("fork");

	if (!pid) {
		dup2(pd[1], STDERR_FILENO);
		close(pd[0]);
		close(pd[1]);
		execv("/usr/bin/gcc", compile_params);
		die("execv");
	}
	else {
		close(pd[1]);
		/* Read from pipe until it is closed */
		while ((out = read(pd[0], buffer, BUFSIZ)) > 0){
			printf("%s", buffer);
		}
		//wait(&status);

		pmpi_distribute_source(atoi(argv[2]), source_code);
		//pmpi_send_source("146.186.64.243", source_code);
		//if (!fork()){
			//execl("./prog", "prog", NULL);
			//die("execl");
		//}
		//else{
			//wait(&status);
		//}
		wait(&status);
	}
	free((char *)source_code);	
	return 0;
}
