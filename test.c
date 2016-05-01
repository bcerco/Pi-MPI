#include <stdio.h>
#include <string.h>
#include "pmpi.h"

int
main(void)
{
	int rank;
	char mes[256];
	pmpi_init(&rank);

	if (!rank){
		printf("I'm the master.\n");
		pmpi_recv_msg(&mes, PMPI_CHAR, sizeof(mes), 1);
		printf("%s\n",mes);
	}
	else{
		printf("I am slave %d.\n", rank);
		sprintf(mes, "%d", 1234);
		pmpi_send_msg(&mes, PMPI_CHAR, sizeof(mes), rank, 0);
	}
}
