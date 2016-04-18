#include <unistd.h>

#ifndef PMPI_H_
#define PMPI_H_

/* pmpi functions */
const int PMPI_INT = 0;
const int PMPI_CHAR = 1;
const int PMPI_FLOAT = 2;

void pmpi_send_source(char *host_address, char *file_name);
void pmpi_recv_source(void);
void pmpi_compile(char *file_name);
void pmpi_run(void);
void pmpi_recv_msg(void *buf, int type, int size);
void pmpi_send_msg(void *buf, int type, int size);

extern int pmpi_comm_pd[2];
extern int pmpi_msg_pd[2];

int create_pipe(void);
#endif
