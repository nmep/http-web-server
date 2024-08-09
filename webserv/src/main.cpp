#include "Server.hpp"

// #include <cstring>

int	main(int ac, char **av)
{
	if (ac == 1)
	{
		// faire le serveur web avec une config par defaut
		std::cout << "default webserv" << std::endl;
	}
	else if (ac == 2)
	{
		// faire le serveur web avec le fichier de config
		Server	serv;
		const std::string file(av[1]);
		// parser le fichier et recuperer ses infos
		if (!serv.parseConfFile(file)) {
			std::cerr << "Error config file" << std::endl;
			return 2;
		}
		// config du serv
		std::cout << serv << std::endl;

		// lancer le serveur
	}
	else
	{
		std::cerr << "Usage: ./webserv [config file]" << std::endl;
		return 2;
	}
	// tester getserver name si l'index est trop grand il est sence renvoyer le dernier elements
}