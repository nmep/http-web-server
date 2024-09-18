#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

#define PORT 8080

int	main()
{
	int client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr) == -1) {
		std::cerr << "error on ip convertion " << std::endl;
		close(client_fd);
		return 1;
	}

	if (connect(client_fd, (const sockaddr *) &addr, sizeof(addr)) == -1) {
		std::cerr << "error on ip convertion " << std::endl;
		close(client_fd);
	}
	
}