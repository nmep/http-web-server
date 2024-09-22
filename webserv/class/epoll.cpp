#include "Socket.hpp"

int Socket::launchEpoll() {
	std::cout << "boucle infinie" << std::endl;
	while (KAA) {
		// verifier s'il y a une nouvelle connexion sur un des serveurs
		for (int i = 0; i < this->portListeningLen; i++) {
			this->sockets[i].clientFd = accept(this->sockets[i].listenFd,
				(sockaddr *) &this->sockets[i].addr, &this->sockets[i].addrLen);
			// connexion marche po
			if (this->sockets[i].clientFd == -1) {
				std::cout << "Errno = " << errno << std::endl;
				if (errno == EWOULDBLOCK) {
					std::cout << RED << "on port " << this->portListening[i] << " no attemp of connexion..." << RESET << std::endl;
				}
				else
				{
					std::cout << YELLOW << "accept sur port " << this->portListening[i] << " pas content" << RESET << std::endl;
					sleep(1);
				}
			}
			else {
				std::cout << GREEN << "port " << this->portListening[i]  << " connexion recue" << std::endl;
				sleep(2);
			}

		}
	}
	return 0;
}