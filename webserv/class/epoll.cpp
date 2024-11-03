#include "Epoll.hpp"
#include "Answer.hpp"
#include "Asynchrone.hpp"

int Epoll::launchEpoll(Configuration const & conf) {
	struct epoll_event	ev, events[MAX_EVENTS];
	std::cout << "port listening len = " << this->portListeningLen << std::endl;
	Asynchrone asynch(this->portListeningLen);

	this->epfd = epoll_create(1);
	if (this->epfd == -1) {
		std::cerr << "Epoll create Error: " << strerror(errno) << std::endl;
		return 0;
	}
	ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR;
	// si ici ca marche pas je fais quoi ?
	for (int i = 0; i < this->portListeningLen; i++) {
		ev.data.fd = this->sockets[i].listenFd;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->sockets[i].listenFd, &ev) == -1) {
			std::cerr << "Epoll ctl failed sur socket " << this->sockets[i].listenFd << ": " << strerror(errno) << std::endl;
			break ;
		}
	}

	int	serverConnxionReceivedId; // il sert a avoir l'index du serv qui a recu une connexion
	int tmpResetSocket = 0;
	while (g_loop) {
		std::cout << "epollwait" << std::endl;
		// usleep(100000);
		this->nfd = epoll_wait(this->epfd, events, MAX_EVENTS, 10000);
		// std::cout << "this nfd = " << this->nfd << std::endl;
		if (this->nfd == -1) {
			std::cout << "bla bla" << std::endl;
			std::cerr << "Epoll wait error: " << strerror(errno) << std::endl;
			break ;
		}

		std::cout << "nfd = " << this->nfd << std::endl;
		for (int i = 0; i < this->nfd; i++) {
			if (events[i].events & EPOLLRDHUP) {
				std::cout << "EPOLLRHDUP detecte" << std::endl;
				closeConnexion(events[i].data.fd);
			}
			else if (events[i].events & EPOLLERR) {
				std::cout << "EPOLLERR detecte" << std::endl;
				closeConnexion(events[i].data.fd);
			}
			else if (events[i].events & EPOLLOUT) {
				// std::cout << "ICI J'ECRIS" << std::endl;
				asynch.Server_action(conf, this->fdAndServer[events[i].data.fd], events[i].data.fd, this->fdAndServerConfIdx[events[i].data.fd]);
				if (asynch.Answers_instances[this->fdAndServer[events[i].data.fd]].GetStatus() == 4)
				{
					asynch.Answers_instances[ this->fdAndServer[events[i].data.fd]].SetStatus(0);
					tmpResetSocket = this->fdAndServer[events[i].data.fd];
					closeConnexion(asynch.Answers_instances[this->fdAndServer[events[i].data.fd]].GetSocketFd());
					asynch.Answers_instances[tmpResetSocket].SetSocketFd(-2);
				}
			}
			else if (events[i].events & EPOLLIN)
			{
				serverConnxionReceivedId = -1;
				serverConnxionReceivedId = isAnServerFd(events[i].data.fd);
				if (serverConnxionReceivedId != -1)
					accept_and_save_connexion(serverConnxionReceivedId, events[i].data.fd);
			}
			else
			{
				std::cout << "autre chose" << std::endl;
				// fermeture de connexion netoyage de ce fd
				closeConnexion(events[i].data.fd);
			}
		}
	}
	this->nfd = epoll_wait(this->epfd, events, MAX_EVENTS, 0);
	std::cout << "fin nfd = " << this->nfd << std::endl;
	sleep(2);
	for (int i = 0; i < this->nfd; i++) {
		closeConnexion(events[i].data.fd);
	}
	for (int i = 0; i < this->portListeningLen; i++)
	{
		if (epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->sockets[i].listenFd, NULL) == -1) {
			std::cerr << "Epoll ctl del failed: " << strerror(errno) << std::endl;
			break ;
		}
		close(this->sockets[i].listenFd);
	}
	std::cout << "EPOLL INSTANCE FD = " << this->epfd << std::endl;
	int qwer = close(this->epfd);
	std::cout << "close = " << qwer << std::endl;
	std::cout << "on est plus dans la boucle" << std::endl;
	return 1;
}
