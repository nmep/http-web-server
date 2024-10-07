#include "Socket.hpp"
#include "Answer.hpp"
#include "Asynchrone.hpp"

int Socket::launchEpoll(Configuration const & conf) {
	struct epoll_event	ev, events[MAX_EVENTS];
	Asynchrone asynch(this->portListeningLen);

	this->epfd = epoll_create(1);
	if (this->epfd == -1) {
		std::cerr << "Epoll create Error: " << strerror(errno) << std::endl;
		return 0;
	}
	for (int i = 0; i < this->portListeningLen; i++) {
		ev.events = EPOLLIN;
		ev.data.fd = this->sockets[i].listenFd;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->sockets[i].listenFd, &ev) == -1) {
			std::cerr << "Epoll ctl failed sur socket " << this->sockets[i].listenFd << ": " << strerror(errno) << std::endl;
			return 0;
		}
	}

	int	serverConnxionReceivedId; // il sert a avoir l'index du serv qui a recu une connexion

	while (KAA) {
		std::cout << "epollwait" << std::endl;
		sleep(1);
		this->nfd = epoll_wait(this->epfd, events, MAX_EVENTS, 0); // TO DO timout voir fichier de configuration
		if (this->nfd == -1) {
			std::cerr << "Epoll wait error: " << strerror(errno) << std::endl;
			return 0;
		}
		serverConnxionReceivedId = -1;
		for (int i = 0; i < this->nfd; i++) {
			// connexion a un serveur sinon c'est un connexion cliente
			serverConnxionReceivedId = isAnServerFd(events[i].data.fd);
			if (serverConnxionReceivedId != -1) {
				std::cout << "j'accepte et j'ajoute" << std::endl;
				accept_and_save_connexion(serverConnxionReceivedId);
				serverConnxionReceivedId = -1;
			}
			else if (events[i].events & EPOLLIN || events[i].events & EPOLLOUT)
			{
				if (events[i].events & EPOLLOUT)
					std::cout << "ICI J'ECRIS" << std::endl;
				else {
					std::cout << "2 event[" << i << "] = " << events[i].data.fd << std::endl;
					std::cout << "EPOLLIN detecte je repond" << std::endl;
				}
				sleep(1);
				// attention a l'appel de la map si un fd n'est pas dans la map il se passe quoi ?
				std::cout << "get = " << this->getFdAndServer(events[i].data.fd) << std::endl;
				asynch.Server_action(conf, this->fdAndServer[events[i].data.fd], events[i].data.fd);
			}
			else if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				std::cout << "EPOLLRHDUP detecte" << std::endl;
				// fermeture de connexion netoyage de ce fd
			}
			else if (events[i].events & EPOLLERR) {
				std::cout << "EPOLLERR detecte" << std::endl;
			}
			else
			{
				std::cout << "autre chose" << std::endl;
			}
		}
	}
	return 1;
}