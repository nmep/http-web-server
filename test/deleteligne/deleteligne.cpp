#include <iostream>
#include <algorithm>
// #include <cstdio>

void	removeLine(std::string & source) {
	std::cout << "avant " << std::endl << source << std::endl;
	size_t end = source.find('\n');
	std::cout << "end = " << end << std::endl;
	
	source.erase(0, end + 1);

	std::cout << "apres\n\n\n" << std::endl;

	std::cout << source << std::endl;
}

int	main()
{
	std::string	str = "Fin de ReadRequest \n\
Fin de Server_action, serv numero 1, nouveau status 2\n\
epollwait\n\
^Cnfd = 1\n\
events[i].data.fd 7\n\
ici c soit un serveur soit une requete qui est lu\n\
get = 1\n\
Debut de Server_action, serv numero 1, socket_fd 7\n\
Debut de WriteFile\n\
Fin de WriteFile\n\
Fin de Server_action, serv numero 1, nouveau status 2\n\
on est plus dans la boucle\n\
Socket Destructeur called\n\
finish\n\
Configuration destructor called\n\
Server destructor called\n\
Location destructeur de location id 7 called";

	removeLine(str);
}