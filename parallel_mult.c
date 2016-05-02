#include <stdio.h>
#include <string.h>
#include "pmpi.h"

int
main(void)
{
	int rank, slaveSum = 1, masterSum = 1, i;
	pmpi_init(&rank);

	if (!rank){
		printf("I'm the master.\n");
		for (i = 1; i <= 3; i++){
			masterSum = masterSum * i;
		}
		for (i = 1; i < 4; i++){
			pmpi_recv_msg(&slaveSum, PMPI_INT, sizeof(slaveSum), i);
			masterSum = masterSum * slaveSum;
		}
		printf("Result of 12!: %d\n", masterSum);
	}
	else{
		printf("I am slave %d.\n", rank);
		for (i = ((rank * 3) + 1); i <= ((rank * 3) + 3); i++){
			slaveSum = slaveSum *  i;
		}
		pmpi_send_msg(&slaveSum, PMPI_INT, sizeof(slaveSum), rank, 0);
	}
}
