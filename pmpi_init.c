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
	int count = 0;
	FILE *f_host;
	FILE *f_node;
	size_t len = 0;
	ssize_t read;
	char * line = NULL;
	char * node_addr = NULL;
	
	/* open the file that contains this nodes IP address */
	f_node = fopen("node_add.txt", "r");
	if (f_node == NULL){
		fprintf(stderr, "Error opening node_add.txt --> %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* get the address of the node calling this function */
	while ((read = getline(&node_addr, &len, f_node)) != -1){
		node_addr[strlen(node_addr) -1] = '\0';
	}
	fclose(f_node);
	len = 0;
	
	/* open the file that contains the addresses of all nodes */
	f_host = fopen("hosts.txt", "r");
	if (f_host == NULL){
		fprintf(stderr, "Error opening host.txt --> %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* set the rank of the node calling this function based on its ordered 
	 * appearance in the hosts file */
	while ((read = getline(&line, &len, f_host)) != -1) {
		line[strlen(line) -1] = '\0';
		if (strcmp(line, node_addr) == 0){
			*rank = count;
		}
		count++;
	}
	fclose(f_host);
}
