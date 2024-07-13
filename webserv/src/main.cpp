#include "Server.hpp"

#include <cstring>

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
		if (!serv.ft_parse_config_file(file))
			std::cerr << "Error config file" << std::endl;
		std::cout << "port = " << serv.GetPort() << std::endl;

		std::cout << "server name = " << serv.GetServerName(1) << std::endl;
		std::cout << "server name = " << serv.GetServerName(2) << std::endl;
		std::cout << "server name = " << serv.GetServerName(3) << std::endl;
		std::cout << "server name = " << serv.GetServerName(4) << std::endl;
		// std::cout << "server name = " << serv.GetServerName(5) << std::endl;




		// parser le fichier et recuperer ses infos
		// lancer le serveur
	}
	else
	{
		std::cerr << "Usage: ./webserv [config file]" << std::endl;
		return 2;
	}
	// tester getserver name si l'index est trop grand il est sence renvoyer le dernier elements
}