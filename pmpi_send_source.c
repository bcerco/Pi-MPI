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

#define PORT 					6665	
#define FILE_TO_SEND	"foo.c"


void
pmpi_send_source(char *host_address, char *file_name)
{
  socklen_t sock_len;
  ssize_t len;
  int ack = 0;
  int orig_sock;
  int fd;
  int sent_bytes = 0;
  char buffer[BUFSIZ];
  char file_size[256];
  off_t offset;
  int remain_data;

  memset(&buffer, 0, sizeof(buffer));
  memset(&file_size, 0, sizeof(file_size));
  struct sockaddr_in
    serv_adr;

  struct hostent *host;

  struct stat file_stat;

  host = gethostbyname(host_address);

  if (host == (struct hostent *) NULL) {
    perror("gethostbyname ");
    exit(2);
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
  /* open file */
  fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Error opening file --> %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
  /* Get file stats */
  if (stat(file_name, &file_stat) != 0){
    fprintf(stderr, "Error fstat --> %s", strerror(errno));
    close(fd);
    exit(EXIT_FAILURE);
  }
  /* output file size */
  fprintf(stdout, "File size: %d bytes\n", (int)file_stat.st_size);
  sprintf(file_size, "%d", (int)file_stat.st_size);
  sock_len = sizeof(struct sockaddr_in);
  
  /* send file size */
  len = send(orig_sock, file_size, sizeof(file_size), 0);
  if (len < 0){
    fprintf(stderr, "Error sending file size --> %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
  /* wait for ack */
  recv(orig_sock, &ack, sizeof(ack), 0);
  
  /* send file name */
  len = send(orig_sock, file_name, strlen(file_name), 0);
  if (len < 0){
    fprintf(stderr, "Error sending file size --> %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
  /* wait for ack */
  recv(orig_sock, &ack, sizeof(ack), 0);
  
  /* send file in chunks */
  offset = 0;
  remain_data = file_stat.st_size;
  while (((sent_bytes = sendfile(orig_sock, fd, &offset, BUFSIZ)) > 0)
      &&
      ((remain_data > 0))){
    remain_data -= sent_bytes;
  }
  close(fd);
  close(orig_sock);
}
