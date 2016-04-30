#include <stdio.h>
#include "pmpi.h"

int
main(void)
{
	int rank;
	pmpi_init(&rank);

	if (!rank){
		printf("I'm the master.\n");
	}
	else{
		printf("I am slave %d.\n", rank);
	}
}
