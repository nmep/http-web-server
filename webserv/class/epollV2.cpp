#include "Socket.hpp"

int Socket::launchEpoll(Configuration const & conf) {
	struct epoll_event	ev, events[MAX_EVENTS];

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

	// j'arrive pas a mettre des condition sur l'event de data pour directement savoir je dois en faire quoi
	while (KAA) {
		this->nfd = epoll_wait(this->epfd, events, MAX_EVENTS, 100); // timout voir fichier de configuration
		if (this->nfd == -1) {
			std::cerr << "Epoll wait error: " << strerror(errno) << std::endl;
			return 0;
		}

		std::cout << "nfd = " << nfd << std::endl;
		sleep(1);
		for (int i = 0; i < this->nfd; i++) {
			// connexion a un serveur sinon c'est un connexion cliente
			if (isAnServerFd(events[i].data.fd)) {
			// if (events[i].events & EPOLLIN) {
				std::cout << "evenements de connexion recu" << std::endl;
				std::cout << "fd de connexion = " << events[i].data.fd << std::endl;

				accept_and_save_connexion(events[i].data.fd);
			}
			else
			{
				std::cout << "je dois repondre a la requete fd = " << events[i].data.fd << std::endl;
				(void) conf;
				if (epoll_ctl(this->epfd, EPOLL_CTL_DEL, events[i].data.fd, &ev) == -1) {
					std::cerr << "Epoll ctl failed sur socket " << this->sockets[i].listenFd << ": " << strerror(errno) << std::endl;
					return 0;
				}
			}

		}
	}
	return 1;
}