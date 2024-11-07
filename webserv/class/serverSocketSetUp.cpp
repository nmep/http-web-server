#include "Epoll.hpp"

Epoll::Epoll()
{
	//std::cout  << YELLOW << "Socket Constructeur called" << RESET << std::endl;
	this->portListening = NULL;
	this->sockets = NULL;
	// this->portListeningLen = 0; sert a rien jsp pk
}

// Epoll::Socket(Socket const & copy) {}

Epoll::~Epoll()
{
	if (this->sockets != NULL)
		delete[] this->sockets;
	if (this->portListening)
		delete[] this->portListening;
}

// renvoie le fd su serveur associe a "fd" sinon renvoie -1
int	Epoll::getFdAndServer(int fd) {
	if (this->fdAndServer.find(fd) != this->fdAndServer.end())
		return this->fdAndServer[fd];
	return -1;
}

// Socket& Epoll::operator=(Socket const & rhs) {}

static bool	checkIfPortIsSet(int *SokcetPort, int value, int length) {
	for (int i = 0; i < length; i++) {
		if (value == SokcetPort[i])
			return true;
	}
	return false;
}

static int assignPortToServerIdx(Configuration const & conf, int Port) {
	for (int i = 0; i < conf.getNbServer(); i++) {
		std::vector<uint16_t>::const_iterator it = conf.getServer(i).GetPortVector().begin();
		for (; it != conf.getServer(i).GetPortVector().end(); it++) {
			if (*it == Port)
				return i;
		}
	}
	return -1;
}

int	Epoll::initAllSockets(Configuration const & conf) {

	// faire un tableau de max port dispo pour y mettre les ports
	this->portListening = new int [conf.getNbPort()];

	this->portListeningLen = 0;
	// remplir un tableau de port a mettre sur ecoute
	for (int i = 0; i < conf.getNbServer(); i++) {
		for (size_t portVectorIndex = 0; portVectorIndex < conf.getServer(i).GetPortVector().size(); portVectorIndex++) {
			if (!checkIfPortIsSet(this->portListening, *(conf.getServer(i).GetPortVector().begin() + portVectorIndex), this->portListeningLen)) {
				this->portListening[this->portListeningLen] = *(conf.getServer(i).GetPortVector().begin() + portVectorIndex);
				this->portListeningLen++;
			}
		}
	}

	this->sockets = new t_socket [this->portListeningLen];

	// // creer une socket pour chaque port
	for (int i = 0; i < this->portListeningLen; i++) {
		if (!initOneSocket(&this->sockets[i], this->portListening[i]))
			return 0;
		this->sockets[i].serverIdx = assignPortToServerIdx(conf, this->portListening[i]);
	}
	return 1;
}

/*
	init une socket qui sera place dans socketStruct qui sera bind au port 'port'
*/
int	Epoll::initOneSocket(t_socket *socketStruct, int port)
{
	// creation de socket
	// Il est vrmt to bo ce garfi je me le ferais bien au petit dejeuner
	socketStruct->listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketStruct->listenFd == -1) {
		std::cerr << "Error while creating socket " << strerror(errno) << std::endl;
		return 0;
	}
	socketStruct->addr.sin_family = AF_INET;
	socketStruct->addr.sin_addr.s_addr = INADDR_ANY;
	socketStruct->addr.sin_port = htons(port);

	socketStruct->addrLen = (socklen_t) sizeof(socketStruct->addr);

	std::memset(socketStruct->addr.sin_zero, '\0', sizeof(socketStruct->addr.sin_zero));

	// socket non bloquante
	// 	je recupere le flag de la socket pour l'utiliser ensuite
	socketStruct->socketFlag = fcntl(socketStruct->listenFd, F_GETFL, 0);
	if (socketStruct->socketFlag == -1) {
		std::cerr << "Error while creating non blocking socket " << strerror(errno) << std::endl;
		return 0;
	}

	if (fcntl(socketStruct->listenFd, F_SETFL, socketStruct->socketFlag | O_NONBLOCK) == -1) {
		std::cerr << "Error while creating non blocking socket " << strerror(errno) << std::endl;
		return 0;
	}

	int opt = 1;
	if (setsockopt(socketStruct->listenFd, SOL_SOCKET, SO_REUSEADDR, &opt , sizeof(int)) == -1) {
		std::cerr << "Error while set reuse socket: " << strerror(errno) << std::endl;
		return 0;
	}

	if (setsockopt(socketStruct->listenFd, SOL_SOCKET, SO_REUSEPORT, &opt , sizeof(int)) == -1) {
		std::cerr << "Error while set reuse socket: " << strerror(errno) << std::endl;
		return 0;
	}

	if (bind(socketStruct->listenFd, (const sockaddr *) &socketStruct->addr, sizeof(socketStruct->addr)) == -1) {
		std::cerr << "Error while binding socket " << strerror(errno) << std::endl;
		close (socketStruct->listenFd);
		return 0;
	}

	if (listen(socketStruct->listenFd, 2) == -1) { // deux connection max sont accepte s'il y en a plus alors elles seront refuse
		std::cerr << "Error while listening socket " << strerror(errno) << std::endl;
		close(socketStruct->listenFd);
		return 0;
	}
	return 1;
}

int	Epoll::accept_and_save_connexion(int servID, int sockFD) {
	int	new_connexion;
	struct epoll_event	ev;

	new_connexion = accept(sockFD, \
		(sockaddr *) &this->sockets[servID].addr, &this->sockets[servID].addrLen);
	if (new_connexion == -1) {
		std::cerr << "Accept failed on serveur n " << servID << ": " << strerror(errno) << std::endl;
		return 0;
	}
	this->fdAndServer[new_connexion] = servID;
	this->fdAndServerConfIdx[new_connexion] = this->sockets[servID].serverIdx;
	// setNonBlockSocket(new_connexion);
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = new_connexion;
	if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, new_connexion, &ev) == -1) {
		std::cerr << "Epoll ctl failed sur socket " << sockFD << ": " << strerror(errno) << std::endl;
		return 0;
	}
	return 1;
}

int	Epoll::setNonBlockSocket(int socket) {
	int socketFlag;

	socketFlag = fcntl(socket, F_GETFL, 0);
	if (socketFlag == -1) {
		std::cerr << "Error while creating non blocking socket " << strerror(errno) << std::endl;
		return 0;
	}
	if (fcntl(socket, F_SETFL, socketFlag | O_NONBLOCK) == -1) {
		std::cerr << "Error while creating non blocking socket " << strerror(errno) << std::endl;
		return 0;
	}
	return 1;
}

int	Epoll::isAnServerFd(int fd) {
	for (int i = 0; i < this->portListeningLen; i++) {
		if (this->sockets[i].listenFd == fd) {
			// return this->sockets[i].serverIdx;
			return i;
		}
	}
	return -1;
}

bool	Epoll::closeConnexion(int fd)
{
	this->fdAndServer.erase(fd);
	this->fdAndServerConfIdx.erase(fd);
	if (epoll_ctl(this->epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		std::cerr << "Epoll ctl del failed: " << strerror(errno) << std::endl;
		return false;
	}
	close(fd);
	return true;
}