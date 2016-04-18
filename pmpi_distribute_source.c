#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "pmpi.h"

void
pmpi_distribute_source(int nodes, char *source)
{
	int count = 0;
	FILE *f;
	size_t len = 0;
	ssize_t read;
	char *line = NULL;
	f = fopen("hosts.txt", "r");
	if (f == NULL)
		exit(EXIT_FAILURE);
	while((read = getline(&line, &len, f)) != -1
			&& count < nodes){
		line[strlen(line) -1] = '\0';
		if (strcmp(line, master) != 0){
			pmpi_send_source(line, source);
			count++;
		}
	}
	fclose(f);
}
