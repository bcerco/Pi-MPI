#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <netdb.h>

#define PORT 6662

void
pmpi_send_msg_direct(char *host_address, char *message)
{
	int orig_sock;
	int len = 0;
	
	struct sockaddr_in
		serv_adr;

	struct hostent *host;

	host = gethostbyname(host_address);

	if (host == (struct hostent *) NULL) {
		perror("gethostbyname");
		exit(EXIT_FAILURE);
	}
	memset(&serv_adr, 0, sizeof(serv_adr));

	serv_adr.sin_family = AF_INET;

	memcpy(&serv_adr.sin_addr, host->h_addr, host->h_length);

	serv_adr.sin_port = htons(PORT);

  if ((orig_sock = socket(AF_INET, SOCK_STREAM,0)) < 0) {
    fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (connect(orig_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) < 0) {
    fprintf(stderr, "Error on connect --> %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
	len = send(orig_sock, message, strlen(message), 0);
  if (len < 0){
    fprintf(stderr, "Error sending file size --> %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
	close(orig_sock);
}
