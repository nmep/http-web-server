#include "Server.hpp"

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
		try
		{
			serv.ft_parse_config_file(file);
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << '\n';
			return 2;
		}
		// parser le fichier et recuperer ses infos
		// lancer le serveur
	}
	else
	{
		std::cerr << "Usage: ./webserv [config file]" << std::endl;
		return 2;
	}
}