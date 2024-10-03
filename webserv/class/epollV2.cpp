#include "Socket.hpp"

int Socket::launchEpoll(Configuration const & conf) {
	int i = 1;
	std::cout << "dans epoll sur le serveur a l'index " << i << std::endl;
	std::cout << "affichage" << std::endl;
	if (conf.getServer(i).getLocation("/"))
		printVector(conf.getServer(i).getLocation("/")->getIndex(), std::cout);
	else
	{
		std::cout << "location / n'existe pas" << std::endl;
	}

	std::cout << "affichage avec test\n\n\n" << std::endl;
	std::cout << "1" << std::endl;
	Server test = Server(conf.getServer(i));
	std::cout << "2" << std::endl;
	// *test = conf.getServer(i);
	std::cout << "3" << std::endl;

	std::cout << "serv 1" << std::endl;
	std::cout << conf.getServer(i) << std::endl;
	
	std::cout << "test" << std::endl;
	std::cout << test << std::endl;

	// delete test; // si c'est un pointeur

	// struct epoll_event	ev, events[MAX_EVENTS];

	// (void)conf;
	// std::cout << "JE SUIS DANS EPOLL" << std::endl;
	// std::cout << conf.getServer(1).getLocation("qwer") << std::endl;

	// this->epfd = epoll_create(1);
	// if (this->epfd == -1) {
	// 	std::cerr << "Epoll create Error: " << strerror(errno) << std::endl;
	// 	return 0;
	// }
	// for (int i = 0; i < this->portListeningLen; i++) {
	// 	ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLHUP;
	// 	ev.data.fd = this->sockets[i].listenFd;
	// 	if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->sockets[i].listenFd, &ev) == -1) {
	// 		std::cerr << "Epoll ctl failed sur socket " << this->sockets[i].listenFd << ": " << strerror(errno) << std::endl;
	// 		return 0;
	// 	}
	// }

	// int	serverConnxionReceivedId;
	// // j'arrive pas a mettre des condition sur l'event de data pour directement savoir je dois en faire quoi

	// // 
	// while (KAA) {
	// 	this->nfd = epoll_wait(this->epfd, events, MAX_EVENTS, 0); // timout voir fichier de configuration
	// 	if (this->nfd == -1) {
	// 		std::cerr << "Epoll wait error: " << strerror(errno) << std::endl;
	// 		return 0;
	// 	}
	// 	serverConnxionReceivedId = -1;
	// 	for (int i = 0; i < this->nfd; i++) {
	// 		// connexion a un serveur sinon c'est un connexion cliente
	// 		serverConnxionReceivedId = isAnServerFd(events[i].data.fd);
	// 		if (serverConnxionReceivedId != -1) {
	// 			std::cout << "j'accepte et j'ajoute" << std::endl;
	// 			accept_and_save_connexion(serverConnxionReceivedId);
	// 			serverConnxionReceivedId = -1;
	// 		}
	// 		else if (events[i].events & EPOLLIN)
	// 		{
	// 			std::cout << "EPOLLIN detecte je repond" << std::endl;

	// 			(void) conf;
	// 			if (epoll_ctl(this->epfd, EPOLL_CTL_DEL, events[i].data.fd, &ev) == -1) {
	// 				std::cerr << "Epoll ctl failed sur socket " << this->sockets[i].listenFd << ": " << strerror(errno) << std::endl;
	// 				return 0;
	// 			}
	// 		}
	// 		else if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
	// 			std::cout << "EPOLLRHDUP detecte" << std::endl;
	// 			// fermeture de connexion netoyage de ce fd
	// 		}
	// 		else if (events[i].events & EPOLLERR) {
	// 			std::cout << "EPOLLERR detecte" << std::endl;
	// 		}
	// 		else
	// 		{
	// 			std::cout << "autre chose" << std::endl;
	// 		}
	// 	}
	// }
	return 1;
}