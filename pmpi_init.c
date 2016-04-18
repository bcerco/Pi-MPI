#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "pmpi.h"

void
pmpi_init(int *rank)
{
	printf("%s\n", this_add);
	int count = 0;
	FILE *f;
	size_t len = 0;
	ssize_t read;
	char * line = NULL;
	f = fopen("hosts.txt", "r");
	if (f == NULL)
		exit(EXIT_FAILURE);
	while ((read = getline(&line, &len, f)) != -1) {
		line[strlen(line) -1] = '\0';
		if (strcmp(this_add,line) == 0){
			*rank = count;
			return;
		}
		count++;
	}
	exit(EXIT_FAILURE);
}
