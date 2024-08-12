#include "configuration.hpp"

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
		Configuration conf;
		const std::string confFileName(av[1]);

		// lancer la config des serveur
		if (!conf.launchServerConf(confFileName))
			return 2;

	}
	else
	{
		std::cerr << "Usage: ./webserv [config file]" << std::endl;
		return 2;
	}
	// tester getserver name si l'index est trop grand il est sence renvoyer le dernier elements

	return 0;
}