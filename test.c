#include <stdio.h>
#include "pmpi.h"

int
main(void)
{
	int rank;
	pmpi_init(&rank);
	int please = 0;

	if (!rank){
		//pmpi_recv_msg(&please, PMPI_INT, sizeof(please));
		printf("%d\n", please);
	}
	else{
		//int yes = 1;
		//pmpi_send_msg(&rank, PMPI_INT, sizeof(rank), master); 
	}
}
