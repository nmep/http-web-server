#include "Socket.hpp"

int Socket::launchEpoll() {
	this->epfd = epoll_create(1);
	if (this->epfd == -1) {
		std::cerr << "Epoll create Error: " << strerror(errno) << std::endl;
		return 0;
	}
	struct epoll_event	ev[this->portListeningLen], events[this->portListeningLen][MAX_EVENTS];

	while (KAA) {
		for (int i = 0; i < this->portListeningLen; i++) {
			ev[i].events = EPOLLIN | EPOLLET;
			ev[i].data.fd = this->sockets[i].listenFd;
			std::cout << "epfd = " << this->epfd << std::endl;
			std::cout << "client fd = " << this->sockets[i].listenFd << std::endl;
			std::cout << "ev[i] events = " << ev[i].events << std::endl;
			std::cout << "ev[i] data fd (= client fd) = " << ev[i].data.fd << std::endl;

			if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->sockets[i].listenFd, &ev[i]) == -1) {
				std::cerr << "Epoll ctl failed: " << strerror(errno) << std::endl;
				return 0;
			}
			std::cout << "ca bloque ici ?" << std::endl;
			this->nfd = epoll_wait(this->epfd, events[i], MAX_EVENTS, -1); // timout voir fichier de configuration
			std::cout << "EPOLL: nfd = " << this->nfd << std::endl;
			sleep(2);
			for (int n = 0; n < this->nfd; n++) {
					
				if (events[i][n].data.fd == this->sockets[i].listenFd) {
					
					this->sockets[i].clientFd = accept(this->sockets[i].listenFd,
						(sockaddr *) &this->sockets[i].addr, &this->sockets[i].addrLen);

					if (this->sockets[i].clientFd == -1) {
						if (errno == EWOULDBLOCK) {
							std::cout << RED << "on port " << this->portListening[i] << " no attemp of connexion..." << RESET << std::endl;
							sleep(1);
						}
						else
						{
							std::cout << YELLOW << "accept sur port " << this->portListening[i] << " pas content" << RESET << std::endl;
							sleep(1);
						}
					}

					else {
						std::cout << GREEN << "port " << this->portListening[i]  << " connexion recue" << std::endl;
						sleep(1);
					}
				}
			}
		}
	}
	return 1;
}