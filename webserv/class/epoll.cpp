#include "Socket.hpp"

int Socket::launchEpoll() {
	std::cout << "boucle infinie" << std::endl;
	while (KAA) {
		// verifier s'il y a une nouvelle connexion sur un des serveurs
		for (int i = 0; i < this->portListeningLen; i++) {
			// this->sockets[i].addrLen = sizeof(this->sockets[i].addr);
			std::cout << "addr = " << this->sockets[i].addrLen << std::endl;
			this->sockets[i].clientFd = accept(this->sockets[i].listenFd,
				(sockaddr *) &this->sockets[i].addr, &this->sockets[i].addrLen);
			// connexion marche po
			if (this->sockets[i].clientFd == -1) {
				if (this->sockets[i].clientFd == EWOULDBLOCK) {
					std::cout << "on port " << this->portListening[i] << " no attemp of connexion..." << std::endl;
					sleep(1);
				}
				else
				{
					std::cout << "accept sur port " << this->portListening[i] << " pas content" << std::endl;
					sleep(1);
				}
			}
			else {
				std::cout << "port " << this->portListening[i]  << " connexion recue" << std::endl;
			}

		}
	}
	return 0;
}