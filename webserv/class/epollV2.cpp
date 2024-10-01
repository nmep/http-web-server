#include "Socket.hpp"
#include "parse_http.hpp"

int Socket::launchEpoll(Configuration const & conf) {
	struct epoll_event	ev, events[MAX_EVENTS];

	(void)conf;
	std::cout << "JE SUIS DANS EPOLL" << std::endl;
	std::cout << conf.getServer(1).getLocation("qwer") << std::endl;

	this->epfd = epoll_create(1);
	if (this->epfd == -1) {
		std::cerr << "Epoll create Error: " << strerror(errno) << std::endl;
		return 0;
	}
	for (int i = 0; i < this->portListeningLen; i++) {
		ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLHUP;
		ev.data.fd = this->sockets[i].listenFd;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->sockets[i].listenFd, &ev) == -1) {
			std::cerr << "Epoll ctl failed sur socket " << this->sockets[i].listenFd << ": " << strerror(errno) << std::endl;
			return 0;
		}
	}

	int	serverConnxionReceivedId;
	// j'arrive pas a mettre des condition sur l'event de data pour directement savoir je dois en faire quoi

	// 
	while (KAA) {
		this->nfd = epoll_wait(this->epfd, events, MAX_EVENTS, 0); // timout voir fichier de configuration
		if (this->nfd == -1) {
			std::cerr << "Epoll wait error: " << strerror(errno) << std::endl;
			return 0;
		}
		serverConnxionReceivedId = -1;
		for (int i = 0; i < this->nfd; i++) {
			// connexion a un serveur sinon c'est un connexion cliente
			serverConnxionReceivedId = isAnServerFd(events[i].data.fd);
			std::cout << serverConnxionReceivedId << " ?" << std::endl;
			if (serverConnxionReceivedId != -1) {
				std::cout << "j'accepte et j'ajoute" << std::endl;
				accept_and_save_connexion(serverConnxionReceivedId);



				
				serverConnxionReceivedId = -1;
			}
			else if (events[i].events & EPOLLIN)
			{
				std::cout << "EPOLLIN detecte je repond" << std::endl;

				Server serv;// c'est temporaire le temps que tu me passe les vrais classes serveur
				Parse_http http(serv);

				http.HandleOneSocket(events[i].data.fd);

				(void) conf;
				// if (epoll_ctl(this->epfd, EPOLL_CTL_DEL, events[i].data.fd, &ev) == -1) {
				// 	std::cerr << "Epoll ctl failed sur socket " << this->sockets[i].listenFd << ": " << strerror(errno) << std::endl;
				// 	return 0;
				// }
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
			}
		}
	}
	return 1;
}