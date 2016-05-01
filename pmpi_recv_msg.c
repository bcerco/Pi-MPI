#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include "pmpi.h"

void
pmpi_recv_msg(void *buf, int type, int size, int rank)
{
	char *path = NULL;
	/* open the corresponding FIFO for the supplied rank*/
	sprintf(path, "node%d.fifo", rank);
	/* open the FIFO in read only mode */
	int fd = open(path, O_RDONLY);
	switch(type){
		case 0:
			read(fd, (int *)buf, size);
			break;
		case 1:
			read(fd, (char *)buf, size);
			break;
		case 2:
			read(fd, (float *)buf, size);
			break;
		default:
			break;
	}
	close(fd);
}
