#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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
	if (argc < 2){
		fprintf(stderr, "Ussage: %d\n", argc);
    exit(EXIT_FAILURE); 
  }
	source_code = strdup(argv[1]);
  binary_file = strtok(argv[1], ".");
  char *const compile_params[] = {"gcc", source_code, "-o", binary_file, NULL};
	
	/* fork and exec to compile program */
	if ((pid = fork()) == -1)
		die("fork");
	
	if (!pid) {
		dup2(pd[1], STDERR_FILENO);
		close(pd[0]);
		close(pd[1]);
		execv("/usr/bin/gcc", compile_params);
		die("execl");
	}
	else {
		close(pd[1]);
		/* Read from pipe until it is closed */
		while ((out = read(pd[0], buffer, BUFSIZ)) > 0){
			printf("%s", buffer);
		}
		wait(&status);
	}
  free((char *)source_code);	
	return 0;
}