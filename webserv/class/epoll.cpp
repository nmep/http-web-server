#include "Socket.hpp"

int Socket::launchEpoll() {
	// epfd est l'instance d'epoll
	this->epfd = new int[this->portListeningLen];
	struct epoll_event	ev[this->portListeningLen], events[this->portListeningLen][MAX_EVENTS];
	this->nfd = new int[this->portListeningLen];

	// boucle sur tout les port qui ont ete mis sur ecoute pour leurs attribuer une instance poll 
	// et faire un epoll ctl pour dire que si tu recois un une requete tu l'ajoute a epfd
	for (int i = 0; i < this->portListeningLen; i++) {
		this->epfd[i] = epoll_create(1);
		if (this->epfd[i] == -1) {
			std::cerr << "Epoll create n " << i << " Error: " << strerror(errno) << std::endl;
			return 0;
		}
		// epollin et epollet sont la pour indiquier a epollwaitr a quel moment renvoyer les fd pret
		ev[i].events = EPOLLIN | EPOLLET;
		ev[i].data.fd = this->sockets[i].listenFd;
		if (epoll_ctl(this->epfd[i], EPOLL_CTL_ADD, this->sockets[i].listenFd, &ev[i]) == -1) {
			std::cerr << "Epoll ctl failed: " << strerror(errno) << std::endl;
			return 0;
		}
	}

	/*
		lancement de tout les serveurs voici comment ca fonctionne il y aura deux versions

		la premiere:	lancer tout les serveurs un a un et boucle sur toute la liste de chaque serveur 
			probleme, si je suis dans une situation ou j'ai plusieurs serveurs
				si le premier a 10 000 requete et le suivant en a une seule, je vais devoir
				d'abord m'occuper des requetes du premier serveur puis m'occuper de la requete du serveur suivant...
				ca metteras en attente la requete du deuxieme serveur trop longtemps.

		la deuxieme:	lancer aussi tout les serveurs un a un mais au lieu de boucle sur chaque serveurs ET de
						m'occuper de toute les requetes renvoyer par epollwait. Je recupere la liste de fd
						et je la garde en memoire. Ainsi je peux boucler sur un fd par serveur a la fois.
						
						Donc je peux traiter tout les premiere requete de chaque serveurs puis les secondes ect.
						
						A qu'elle moment je dois faire un epoll wait ? Je dois faire un epoll wait
						des que l'instance epoll du serveur x (le taleau qui est rempli par epoll wait contenant tout les fd pret a etre lu)
						a ete lu entierement.

						Mais si je fais ca et que ca prend trop de temps parce que j'ai 10 000 serveurs a traite 
						je ne risque pas de rate des requetes sur un serveur ? Je sais pas on verra plus tard.
						
			Avantage: Asynchrone.
			Desavantage : je dois le faire
	
	*/

	// premiere versions
	while (KAA) {
		// on boucle un a un sur tout les serveurs
		for (int i = 0; i < this->portListeningLen; i++) {

			// debug

			std::cout << "ca bloque ici ?" << std::endl;
			this->nfd[i] = epoll_wait(this->epfd[i], events[i], MAX_EVENTS, 0); // timout voir fichier de configuration
			// debug
			std::cout << "le port qui est en ecoute est " << this->portListening[i] << std::endl;
			std::cout << "dans epoll port " << this->portListening[i] << " fd = " << this->sockets[i].listenFd << std::endl;
			sleep(1);
			//
			for (int n = 0; n < this->nfd[i]; n++) {
				
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