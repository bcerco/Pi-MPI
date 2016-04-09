#include "cbjlocal.h"

int
main(void) 
{
	char error_message[] = "Unknown command.\n\0"; 
	int orig_sock,
			new_sock,
			clnt_len;

	struct sockaddr_in
		clnt_adr,
		serv_adr;

	int len, i;

	if((orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("generate error");
		exit(1);
	}

	memset (&serv_adr, 0, sizeof(serv_adr));

	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(PORT);

	if (bind(orig_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0) {
		perror("bind error");
		close(orig_sock);
		exit(2);
	}

	if (listen(orig_sock, 5) < 0) {
		perror("listen error");
		exit(3);
	}
	do {
		clnt_len = sizeof(clnt_adr);

		if ((new_sock = accept(orig_sock, (struct sockaddr *) &clnt_adr,
						&clnt_len)) < 0) {
			perror("accept error");
			close(orig_sock);
			exit(4);
		}

		if (fork() == 0) {
			while ((len=read(new_sock,buf,BUFSIZ)) > 0) {

				if (strcmp(buf, "hit\n") == 0)
					write(new_sock, buf, len);
				else if (strcmp(buf, "stand\n") == 0)
					write(new_sock, buf, len);
				else if(buf[0] != '.')
					write(new_sock, error_message, strlen(error_message));
				else
					break;
				memset(&buf[0], 0, BUFSIZ);

			}
			close(new_sock);
			exit(0);
		}
		else {
			close(new_sock);
		}
	} while (1);
}
