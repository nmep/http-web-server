#include "Socket.hpp"

int Socket::launchEpoll() {
	// epfd est l'instance d'epoll
	this->epfd = new int[this->portListeningLen];
	// la structure ev permet de definir les evenements qui seront surveille par epoll wait
	// le structure events permet a epoll wait de dire qu'elle evenements on ete repere ( a verifier !!!!!)
	struct epoll_event	ev[this->portListeningLen], events[this->portListeningLen][MAX_EVENTS];
	this->nfd = new int[this->portListeningLen];
	int n; // n sert un index sur l'instance epoll qui est utilise pour lire tout les premiers fd 
	int	nfd_count; // sert a conter le nombre de epoll_wait qui a fonctionne pour alloue ensuite serverData
	int nfd_i; // sert a indexer sur le serveur pour la distribution de fd
	// de chaque serveur (le premier puis le deuxieme ect ect) c'est pour ca qu'il est cree en avance

	// boucle sur tout les port qui ont ete mis sur ecoute pour leurs attribuer une instance epoll (le 1 ne sert a rien)
	// et faire un epoll ctl pour dire que si tu recois une requete tu l'ajoute a epfd 

	// j'en profite aussi pour faire un premier memset a 0 sur la struct < necessaire ?
	for (int i = 0; i < this->portListeningLen; i++) {
		this->epfd[i] = epoll_create(1);
		if (this->epfd[i] == -1) {
			std::cerr << "Epoll create n " << i << " Error: " << strerror(errno) << std::endl;
			return 0;
		}
		// epollin et epollet sont la pour indiquier a epollwaite a quel moment renvoyer les fd pret
		ev[i].events = EPOLLIN;
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
						Pour savoi si ca a ete lu entiere et que je suis a la fin, j'ai trois idees et je ne sais pas si elle marche

							premiere idee, faire un memeset dessus
							deuxieme ajoute un -1 a max events + 1;
							troisieme voir sur epoll wait renvoyer la taille

						Mais si je fais ca et que ca prend trop de temps parce que j'ai 10 000 serveurs a traite 
						je ne risque pas de rate des requetes sur un serveur ? Je sais pas on verra plus tard.
						
			Avantage: Asynchrone.
			Desavantage : je dois le faire
	*/

	// deuxieme version

	// j'alloue une seule fois de taille max parce que je ne peux pas savoir a l'avance le nombre de serveur qui vont recevoir de la connexion
	// je garderais nfd_count comme conteur
	this->nfdReady = new t_nfd_ready[this->portListeningLen];
	while (KAA) {

		// apres un tour de boucle tout les fd on ete traite est ce que c'est necessaire de faire un memset a 0 ou epoll wait s'en charge ?
		// ou encore le kernel mets tout a 0 ? jsp faut voir
		nfd_count = 0;

		for (int i = 0; i < this->portListeningLen; i++) {
			// debug
			std::cout << "socket[" << i << "] fd = " << this->sockets[i].listenFd << std::endl;
			sleep(1);
			//
			this->nfd[i] = epoll_wait(this->epfd[i], events[i], MAX_EVENTS, 0); // timout voir fichier de configuration
			// epollwait crash
			if (this->nfd[i] == -1) {
				std::cerr << "Epoll wait error: " << strerror(errno) << std::endl;
				return 0;
			}
			// epoll wait a detecte des evenements sur events
			// j'attribue les valeurs necessaires a une case de mon tableau nfdReady
			if (this->nfd[i] > 0) {
				this->nfdReady[nfd_count].nfd = this->nfd[i];
				this->nfdReady[nfd_count].listenFD = this->sockets[i].listenFd;
				
				this->nfdReady[nfd_count].nfdReadyEvents = events[i];
				nfd_count++;
			}
			std::cout << "this nfd[" << i << "] = " << this->nfd[i] << std::endl;
			sleep(1);
		}

		if (nfd_count == 0) {
			std::cout << "aucun serveur n'ont rien recu je recommence" << std::endl;
			sleep(1);
			continue ;
		}
		std::cout << "n = " << n << " < this->nfd[" << nfd_i << "] = " << this->nfd[nfd_i] << " ?" << std::endl;
		// est ce que epoll wait a marcher ? epoll wait renvoyer des fd qui sont pret a l'ecoute mais en soit si j'ai 10 000 serveur et
		// qu'il n'y a que le numero 5402 qui est pret ca vas trop boucler pour rien

		n = -1;
		// n commence au debut
		// on boucle un a un sur tout les serveurs qui on recu un reponse de epoll wait (nfd ready TO DO)

		// force epoll wait pour tester

		this->nfd[2] = 5;
		this->nfd[1] = 2;
		this->nfd[0] = 0;

		// ici je boucle sur la liste des fd des serveurs qui ont recu une connexion
		nfd_i = 0;
		while (++n < this->nfdReady[nfd_i].nfd) {

			// ici je boucle sur les serveurs
			nfd_i = 0;
			while (nfd_i <= nfd_count) {

				// si l'index du fd que j'essaye de regarder est plus grand que le nombre de fd recu je passe au serveur suivant
				if (n > this->nfdReady[nfd_i].nfd) {
					nfd_i++;
					continue;
				}
				// permet de savoir l'evennement recu correspond a quel serveur
				if (events[nfd_i][n].data.fd == this->sockets[nfd].listenFd) { // PRENDRE LE NFD READY 
					
					this->sockets[nfd_i].clientFd = accept(this->sockets[nfd_i].listenFd,
						(sockaddr *) &this->sockets[nfd_i].addr, &this->sockets[nfd_i].addrLen);

					if (this->sockets[nfd_i].clientFd == -1) {
						if (errno == EWOULDBLOCK) {
							std::cout << RED << "on port " << this->portListening[nfd_i] << " no attemp of connexion..." << RESET << std::endl;
							sleep(1);
						}
						else
						{
							std::cout << YELLOW << "accept sur port " << this->portListening[nfd_i] << " pas content" << RESET << std::endl;
							sleep(1);
						}
					}

					// il manque un epoll ctl et un conf de epollet a ajouter ici voir man epoll
					else {
						std::cout << GREEN << "port " << this->portListening[nfd_i]  << " connexion recue" << std::endl;
						sleep(1);
					}
				}
				nfd_i++;
			}
		}
	}

	return 1;
}