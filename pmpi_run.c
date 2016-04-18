#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "pmpi.h"
	
void
pmpi_run(void)
{
	int status;

	/* fork and exec to run program */
	if (!fork()){
		execl("./prog", "prog", NULL);
	}
	else{
		wait(&status);
	}
}
