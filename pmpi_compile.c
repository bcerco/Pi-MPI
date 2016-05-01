#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "pmpi.h"
	
void
pmpi_compile(char *file_name)
{
	int status;
	//char *binary_file = strdup(file_name);
	//binary_file = strtok(file_name, ".");
	char *const compile_params[] = 
	{
		"gcc", 
		file_name,
		"pmpi_init.c",
		"pmpi_recv_msg.c",
		"pmpi_send_msg.c",
		"pmpi_send_msg_direct.c",
		"-o", 
		"prog", 
		NULL
	};

	/* fork and exec to compile program */
	if (!fork()){
		execv("/usr/bin/gcc", compile_params);
	}
	else{
		wait(&status);
	}
	//free((char *)binary_file);
}
