#include "Epoll.hpp"
#include "Answer.hpp"
#include "Asynchrone.hpp"

int Epoll::launchEpoll(Configuration const & conf) {
	struct epoll_event	ev, events[MAX_EVENTS];
	Asynchrone asynch(this->portListeningLen);

	this->epfd = epoll_create(1);
	if (this->epfd == -1) {
		std::cerr << "Epoll create Error: " << strerror(errno) << std::endl;
		return 0;
	}
	ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR;
	for (int i = 0; i < this->portListeningLen; i++) {
		ev.data.fd = this->sockets[i].listenFd;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->sockets[i].listenFd, &ev) == -1) {
			std::cerr << "Epoll ctl failed sur socket " << this->sockets[i].listenFd << ": " << strerror(errno) << std::endl;
			break ;
		}
	}

	int	serverConnxionReceivedId;
	int tmpResetSocket = 0;
	while (g_loop) {
		this->nfd = epoll_wait(this->epfd, events, MAX_EVENTS, 10000);
		if (this->nfd == -1) {
			break ;
		}

		for (int i = 0; i < this->nfd; i++) {
			if (events[i].events & EPOLLRDHUP) {
				closeConnexion(events[i].data.fd);
			}
			else if (events[i].events & EPOLLERR) {
				closeConnexion(events[i].data.fd);
			}
			else if (events[i].events & EPOLLOUT) {
				asynch.Server_action(conf, this->fdAndServer[events[i].data.fd], events[i].data.fd, this->fdAndServerConfIdx[events[i].data.fd], \
					this->epfd, this->nfd, events, this->sockets, this->portListeningLen);
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
				closeConnexion(events[i].data.fd);
		}
	}

	this->nfd = epoll_wait(this->epfd, events, MAX_EVENTS, 0);
	for (int i = 0; i < this->nfd; i++)
		closeConnexion(events[i].data.fd);
	for (int i = 0; i < this->portListeningLen; i++)
	{
		if (epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->sockets[i].listenFd, NULL) == -1) {
			break ;
		}
		close(this->sockets[i].listenFd);
	}
	close(this->epfd);
	return 1;
}
