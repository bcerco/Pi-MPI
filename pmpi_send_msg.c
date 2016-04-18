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
pmpi_send_msg(void *buf, int type, int size, char *dest)
{
	switch(type){
		case 0:
			//write(pmpi_msg_pd[1], (int *)buf, size);
			pmpi_send_msg_direct(dest, (int *) buf, size);
			break;
		case 1:
			//write(pmpi_msg_pd[1], (char *)buf, size);
			pmpi_send_msg_direct(dest, (int *) buf, size);
			break;
		case 2:
			//write(pmpi_msg_pd[1], (float *)buf, size);
			pmpi_send_msg_direct(dest, (int *) buf, size);
			break;
		default:
			break;
	}
}
