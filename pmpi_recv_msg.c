#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include "pmpi.h"

void
pmpi_recv_msg(void *buf, int type, int size)
{
	switch(type){
		case PMPI_INT:
			read(pmpi_msg_pd[0], (int *)buf, size);
			break;
		case PMPI_CHAR:
			read(pmpi_msg_pd[0], (char *)buf, size);
			break;
		case PMPI_FLOAT:
			read(pmpi_msg_pd[0], (float *)buf, size);
			break;
		default:
			break;
	}
}
