#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int	main()
{
	int socket_fd;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket creation failed");
		return 2;
	}
	std::cout << "socket fd = " <<  socket_fd << std::endl;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; // j'assigne n'importe quel port de connection mais si c'est un vrais serveur il faudra preciser le port a son protocol
	addr.sin_port = htons(PORT);

	std::memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

	if (bind(socket_fd, (const sockaddr *) &addr, sizeof(addr)) == -1)
	{
		perror("socket binding failed");
		close (socket_fd);
		return 2;
	}
	if (listen(socket_fd, 3) == -1)
	{
		perror("listen error");
		close(socket_fd);
		return 2;
	}

	int accept_socket;
	socklen_t client_len = sizeof(addr);
	while (1)
	{
		// accepte la connection
		printf("j'ecoute...\n");
		if ((accept_socket = accept(socket_fd, (struct sockaddr *)&addr, &client_len)) == -1)
		{
			perror("Accept");
			close(socket_fd);
			return 2;
		}
		// repondre a la connection
		char buffer[1024] = {0};
		int readval = read(accept_socket, buffer, 1024);
		if (readval < 0)
		{
			printf("no bytes are there to read\n");
			continue ;
		}
		const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
//		const char *hello = "Hello world!";
		write(accept_socket, hello, strlen(hello));
		printf("%s\n", buffer);
		close(accept_socket);
	}
	close(socket_fd);

}
