#include "Socket.hpp"

int Socket::launchEpoll() {
	struct epoll_event	ev, events[MAX_EVENTS];

	this->epfd = epoll_create(1);
	if (this->epfd == -1) {
		std::cerr << "Epoll create Error: " << strerror(errno) << std::endl;
		return 0;
	}
	for (int i = 0; i < this->portListeningLen; i++) {
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = this->sockets[i].listenFd;
		std::cout << "serveur " << i << " fd = " << this->sockets[i].listenFd << std::endl;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->sockets[i].listenFd, &ev) == -1) {
			std::cerr << "Epoll ctl failed sur socket " << this->sockets[i].listenFd << ": " << strerror(errno) << std::endl;
			return 0;
		}
	}

	int serv_index;
	while (KAA) {
		this->nfd = epoll_wait(this->epfd, events, MAX_EVENTS, 0); // timout voir fichier de configuration
		// epollwait crash
		if (this->nfd == -1) {
			std::cerr << "Epoll wait error: " << strerror(errno) << std::endl;
			return 0;
		}

		for (int i = 0; i < this->nfd; i++) {
			// permet de savoir l'evennement recu correspond a quel serveur
			serv_index = 0;
			while (events[i].data.fd != this->sockets[serv_index].listenFd && serv_index < this->portListeningLen){
				this->sockets[serv_index].clientFd = accept(this->sockets[serv_index].listenFd,
					(sockaddr *) &this->sockets[serv_index].addr, &this->sockets[serv_index].addrLen);

				if (this->sockets[serv_index].clientFd == -1) {
					if (errno == EWOULDBLOCK) {
						std::cout << RED << "on port " << this->portListening[serv_index] << " no attemp of connexion..." << RESET << std::endl;
						sleep(1);
					}
					if (serv_index == this->portListeningLen)
						serv_index = 0;
				}
				serv_index++;
			}
			// aucun serv renvoyer sur le premier serv
			// il manque un epoll ctl et un conf de epollet a ajouter ici voir man epoll
			ev.events = EPOLLIN | EPOLLET;

			// je repond ici
			std::cout << GREEN << "port " << this->portListening[serv_index]  << " connexion recue" << RESET << std::endl;
			sleep(1);
		}
	}
	return 1;
}