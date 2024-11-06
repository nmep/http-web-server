#include "configuration.hpp"
#include "Epoll.hpp"
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
	Epoll			server;

	signal(SIGQUIT, handle_signal); // ctrl backslash
	signal(SIGINT, handle_signal); // ctrl c
	signal(SIGTSTP, handle_signal); // ctrl z
	signal(SIGTERM, handle_signal); // si on kill notre pid avec kill -15 PID
	signal(SIGHUP, handle_signal); // si on kill notre pid avec kill -1 PID

	if (ac == 1)
	{
		// faire le serveur web avec une config par defaut
		//std::cout  << "default conf" << std::endl;
		const std::string defaultConfFileName("conf/default.conf");
		if (!conf.launchServerConf(defaultConfFileName)) {
			// conf.clearConfiguration();
			//std::cout  << "la conf est pas ok" << std::endl;
			return 2;
		}
		//std::cout  << "la default conf est ok" << std::endl;
	}
	else if (ac == 2)
	{
		//std::cout  << "file gived conf" << std::endl;
		const std::string confFileName(av[1]);
		// lancer la config des serveur
		if (!conf.launchServerConf(confFileName)) {
			//std::cout  << "la conf est pas ok" << std::endl;
			return 2;
		}
		//std::cout  << "la conf est ok" << std::endl;
	}
	else
	{
		std::cerr << "Usage: ./webserv [config file] " << std::endl;
		return 2;
	}

	//std::cout  << conf << std::endl;
	if (!server.initAllSockets(conf)) {
		//std::cout  << "MARCHE PAS" << std::endl;
		return 2;
	}
	server.launchEpoll(conf);

	return 0;
}