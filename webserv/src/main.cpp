#include "Server.hpp"

#include <cstring>

void	printLocation(std::map<std::string, std::map<std::string, std::vector<std::string> > > map);
void	printVector(std::vector<std::string> v);

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
		if (!serv.ft_parse_config_file(file)) {
			std::cerr << "Error config file" << std::endl;
			return 2;
		}
		std::cout << "main" << std::endl;
		std::cout << "port = " << serv.GetPort() << std::endl;
		std::cout << "error page" << std::endl;
		std::cout << serv.GetErrorPage("404") << std::endl;
		std::cout << "client max body size" << std::endl;
		std::cout << serv.GetClientMaxBodySize() << std::endl;
		std::cout << "Location" << std::endl;
		printLocation(serv.GetMap());


		std::cout << "GetLocation directive value " << std::endl;
		
		std::vector<std::string> locationvalue;
		printVector(serv.GetLocationDirectiveValue("weqr", "allowedMethods"));

		locationvalue = serv.GetLocationDirectiveValue("/", "allowedMethods");
		if (locationvalue.size() == 0) {
			std::cout << "location existe pas" << std::endl;
		}
		// lancer le serveur
	}
	else
	{
		std::cerr << "Usage: ./webserv [config file]" << std::endl;
		return 2;
	}
	// tester getserver name si l'index est trop grand il est sence renvoyer le dernier elements
}