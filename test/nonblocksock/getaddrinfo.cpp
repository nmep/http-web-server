#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>

int	main(int ac, char **av)
{
	char hostname[1024];
	hostname[1023] = '\0';

	if (gethostname(hostname, 1023) == -1) {
		std::cerr << "Error getting hostname: " << std::endl;
		return 1;
	}

	std::cout << "hostname = " << hostname << std::endl;

	struct addrinfo hints, *res, *tmp;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(hostname, NULL, &hints, &res);

	if (status != 0) {
		std::cerr << "Error in getaddrinfo: " << gai_strerror(status);
		return 2;
	}

	printf("@ ip for %s:\n\n", av[1]);


	char ipstr[INET_ADDRSTRLEN];

	struct sockaddr_in *ipv4 = (struct sockaddr_in *) res->ai_addr;

	
}