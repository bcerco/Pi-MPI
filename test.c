#include <stdio.h>
#include "pmpi.h"

int
main(void)
{
	int rank;
	int mes;
	pmpi_init(&rank);

	if (!rank){
		printf("I'm the master.\n");
		pmpi_recv_msg(&mes, PMPI_INT, sizeof(mes), 1);
		printf("%d\n", mes);
	}
	else{
		printf("I am slave %d.\n", rank);
		mes = 123;
		pmpi_send_msg(&mes, PMPI_INT, sizeof(mes), rank, 0);
	}
}
