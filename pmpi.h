#ifndef PMPI_H_
#define PMPI_H_

/* pmpi functions */
void pmpi_send_source(char *host_address, char *file_name);
void pmpi_recv_source(void);

static int pmpi_comm_pd[2];

#endif
