#include "configuration.hpp"
#include "Socket.hpp"
#include <csignal>

int g_loop = 1;

void handle_signal(int signal)
{
	if (signal == SIGQUIT || signal == SIGINT || signal == SIGTERM || signal == SIGHUP || signal == SIGTSTP)
		g_loop = 0;
}

int	main(int ac, char **av)
{
	Configuration	conf;
	Socket			socket;

	// si on fait plus de signaux il faudrait peut etre faire une boucle de 0 a X todo
	signal(SIGQUIT, handle_signal); // ctrl backslash
	signal(SIGINT, handle_signal); // ctrl c
	signal(SIGTSTP, handle_signal); // ctrl z
	signal(SIGTERM, handle_signal); // si on kill notre pid avec kill -15 PID
	signal(SIGHUP, handle_signal); // si on kill notre pid avec kill -1 PID

	if (ac == 1)
	{
		// faire le serveur web avec une config par defaut
		std::cout << "default webserv" << std::endl;
		return 0;
	}
	else if (ac == 2)
	{
		const std::string confFileName(av[1]);

		// lancer la config des serveur
		if (!conf.launchServerConf(confFileName)) {
			// conf.clearConfiguration();
			std::cout << "la conf est pas ok" << std::endl;
			return 2;
		}
		std::cout << "la conf est ok" << std::endl;
	}
	else
	{
		// ici je dois utiliser une config de base pour lancer le serveur donc le else n'a pas lui d'etre
		std::cerr << "Usage: ./webserv [config file]" << std::endl;
		return 2;
	}

	// std::cout << conf << std::endl;
	// std::cout << conf << std::endl;
	if (!socket.initAllSockets(conf)) {
		std::cout << "MARCHE PAS" << std::endl;
		return 2;
	}
	socket.launchEpoll(conf);
	// creation de la socket

	return 0;
}