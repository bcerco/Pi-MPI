#include <stdio.h>
#include <string.h>
#include "pmpi.h"

int
main(void)
{
	int rank, slaveSum = 0, masterSum = 0, i;
	pmpi_init(&rank);

	if (!rank){
		printf("I'm the master.\n");
		for (i = 0; i < 2500; i++){
			masterSum += i;
		}
		for (i = 1; i < 4; i++){
			pmpi_recv_msg(&slaveSum, PMPI_INT, sizeof(slaveSum), i);
			masterSum += slaveSum;
		}
		printf("Sum of first 10000 integers: %d\n", masterSum);
	}
	else{
		printf("I am slave %d.\n", rank);
		for (i = (rank * 2500); i < ((rank * 2500) + 2500); i++){
			slaveSum += i;
		}
		if (rank == 3)
			slaveSum += 10000;
		pmpi_send_msg(&slaveSum, PMPI_INT, sizeof(slaveSum), rank, 0);
	}
}
