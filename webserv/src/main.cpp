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
		if (!conf.launchServerConf(confFileName))
			return 2;
		std::cout << "\e[0;31m je suis dans le main.cpp" << "\033[0m" << std::endl;
		if (conf.getServer(0).getLocation("/")) {
			std::cout << "trouve" << std::endl;
			std::cout << *(conf.getServer(0).getLocation("/")) << std::endl;
		}
		else
			std::cout  << "pas trouve" << std::endl;
	}
	else
	{
		std::cerr << "Usage: ./webserv [config file]" << std::endl;
		return 2;
	}
	conf.clearConfiguration();
	return 0;
}