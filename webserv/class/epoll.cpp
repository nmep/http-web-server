#include "Socket.hpp"

int Socket::launchEpoll() {
	this->epfd = epoll_create(0);
	if (this->epfd == -1) {
		std::cerr << "Epoll create Error: " << strerror(errno) << std::endl;
		return 0;
	}
	struct epoll_event	ev, *events;

	while (KAA) {
		this->nfd = epoll_wait(this->epfd, events, MAX_EVENTS, 0);
		std::cout << "EPOLL: nfd = " << this->nfd << std::endl;
		for (int n = 0; n < this->nfd; n++) {
			for (int i = 0; i < this->portListeningLen; i++) {
				
				if (events[n].data.fd == this->sockets[i].listenFd) {
					
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
						ev.events = EPOLLIN | EPOLLET;
						ev.data.fd = this->sockets[i].clientFd;
						if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->sockets[i].clientFd, &ev) == -1) {
							std::cerr << "Epoll ctl failed: " << strerror(errno) << std::endl;
							return 0;
						}
						std::cout << GREEN << "port " << this->portListening[i]  << " connexion recue" << std::endl;
						sleep(1);
					}
				}
			}
		}
	}
	return 1;
}