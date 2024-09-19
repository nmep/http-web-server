#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

// un serveur = deux fd, un pour l'ecoute et un pour le client

int	main()
{
	int	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8080);

	int flag = fcntl(listen_fd, F_GETFL);
	fcntl(listen_fd, F_SETFL, flag | O_NONBLOCK);


	int	yes = 1;

	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		printf("error bind: %s\n", strerror(errno));
		close(listen_fd);
		return 2;
	}

	if (bind(listen_fd, (const sockaddr *) &addr, sizeof(addr)) == -1) {
		printf("error bind: %s\n", strerror(errno));
		close(listen_fd);
		return 2;
	}

	if (listen(listen_fd, 2) == -1) {
		close(listen_fd);
		printf("error listen: %s\n", strerror(errno));
		return 2;
	}

	socklen_t addr_size = sizeof(addr);
	int	cfd;

	while (1) {
		cfd = accept(listen_fd, (struct sockaddr *) &addr, &addr_size);
		if (cfd == -1) {
			if (errno == EWOULDBLOCK) {
				printf("no pending co...\r\n");
				sleep(1);
			}
			else {
				close (listen_fd);
				close(cfd);
				perror("error on accepting connection\n");
				return 1;
			}
		}
		else
		{
			printf("Connection receive !\n");
			char msgToClient[] = "hello from Server";
			char msgFromClient[4096];

			if (recv(cfd, msgFromClient, 4096, 0) == -1) {
				printf("error bind: %s\n", strerror(errno));
				close(listen_fd);
				close(cfd);
				return 2;
			}
			printf("In server socket receive: [%s]\n", msgFromClient);
			send(cfd, msgToClient, sizeof(msgToClient), 0);
			close(cfd);
			break ;
		}
	}
	close(cfd);
	close(listen_fd);
}