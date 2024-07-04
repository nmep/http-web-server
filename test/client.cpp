#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>

#define PORT 8080

int	main()
{
	int socket_fd;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Scoket creation failed");
		return 2;
	}

	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// convertit l'addresse ipv4 en forme binaire pour verifier si elle est correct
	if ((inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)) <= 0)
	{
		perror("invalid address/ Address not suported ");
		close(socket_fd);
		return 2;
	}
	// connect la socket du serveur a l'addresse du serveur; (du coup qu'elle diff il y a entre bind et connect)
	if (connect(socket_fd, (const sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("Connection failed ");
		return 2;
	}
	// envoier le message
	const char *hello = "hello from client";
	send(socket_fd, hello, strlen(hello), 0);
	printf("hello message sent\n");

	// recuperer le message de retour du serveur
	char buffer[1024] = {0};
	int valread = read(socket_fd, buffer, 1024);
	if (valread < 0)
	{
		printf("nothing was read\n");
		return 2;
	}
	printf("%s\n", buffer);
	return 0;

}