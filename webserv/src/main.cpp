#include "configuration.hpp"

// #include <cstring>

int	main(int ac, char **av)
{
	Configuration conf;

	if (ac == 1)
	{
		// faire le serveur web avec une config par defaut
		std::cout << "default webserv" << std::endl;
	}
	else if (ac == 2)
	{
		const std::string confFileName(av[1]);

		// lancer la config des serveur
		if (!conf.launchServerConf(confFileName)) {
			// conf.clearConfiguration();
			return 2;
		}
	}
	else
	{
		// ici je dois utiliser une config de base pour lancer le serveur donc le else n'a pas lui d'etre
		std::cerr << "Usage: ./webserv [config file]" << std::endl;
		return 2;
	}
	
	std::cout << conf << std::endl;

	// conf.clearConfiguration();
	return 0;
}