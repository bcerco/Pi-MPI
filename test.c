#include <stdio.h>
#include <string.h>
#include <time.h>
#include "pmpi.h"

int
main(void)
{
	int rank, slave1, slave2, slave3, curslave;
	char mes[256];
	pmpi_init(&rank);

	if (!rank){
		printf("I'm the master.\n");
		pmpi_recv_msg(&slave1, PMPI_INT, sizeof(slave1), 1);
		printf("%d\n",slave1);
		pmpi_recv_msg(&slave2, PMPI_INT, sizeof(slave2), 2);
		printf("%d\n",slave2);
		pmpi_recv_msg(&slave3, PMPI_INT, sizeof(slave3), 3);
		printf("%d\n",slave3);
	}
	else{
		printf("I am slave %d.\n", rank);
		curslave = 10 * rank;
		pmpi_send_msg(&curslave, PMPI_INT, sizeof(curslave), rank, 0);
		time_t mytime;
		mytime = time(NULL);
		printf("%s\n", ctime(&mytime));
	}
}
