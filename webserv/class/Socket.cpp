#include "Socket.hpp"

Socket::Socket()
{
	std::cout << YELLOW << "Socket Constructeur called" << RESET << std::endl;
	this->portListening = NULL;
	this->sockets = NULL;
	// this->portListeningLen = 0; sert a rien jsp pk
}

// Socket::Socket(Socket const & copy) {}

Socket::~Socket()
{
	std::cout << YELLOW << "Socket Destructeur called" << RESET << std::endl;
	if (this->sockets != NULL)
		delete[] this->sockets;
	if (this->portListening)
		delete[] this->portListening;
}

// Socket& Socket::operator=(Socket const & rhs) {}

static bool	checkIfPortIsSet(int *SokcetPort, int value, int length) {
	for (int i = 0; i < length; i++) {
		std::cout << value << " == " << SokcetPort[i] << std::endl;
		if (value == SokcetPort[i])
			return true;
	}
	return false;
}

/*
	Si il y a deux serveur qui ecoute sur le meme port lors une seule socket sera cree
	je creer un tableau de int avec ecrit un port qui est mit sur ecoute. 
	Si je veux mettre un nouveau port sur ecoute je verfie avant si il ne l'est pas deja
*/
int	Socket::initAllSockets(Configuration const & conf) {
	this->sockets = new t_socket [conf.getNbServer()];

	// faire un tableau de max port dispo pour y mettre les ports
	this->portListening = new int [conf.getNbServer()];

	this->portListeningLen = 0;
	// remplir un tableau de port a mettre sur ecoute
	for (int i = 0; i < conf.getNbServer(); i++) {
		if (!checkIfPortIsSet(this->portListening, conf.getServer(i).GetPort(), this->portListeningLen)) {
			this->portListening[this->portListeningLen] = conf.getServer(i).GetPort();
			this->portListeningLen++;
		}
	}
	// creer une socket pour chaque port
	for (int i = 0; i < this->portListeningLen; i++) {
		if (!initOneSocket(&this->sockets[i], this->portListening[i]))
			return 0;
	}
	return 1;
}

/*
	init une socket qui sera place dans socketStruct qui sera bind au port 'port'
*/
int	Socket::initOneSocket(t_socket *socketStruct, int port)
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

int	Socket::accept_and_save_connexion(int servID) {
	int	new_connexion;
	struct epoll_event	ev;

	std::cout << "serv id = " << servID << std::endl;
	new_connexion = accept(this->sockets[servID].listenFd, \
		(sockaddr *) &this->sockets[servID].addr, &this->sockets[servID].addrLen);
	if (new_connexion == -1) {
		std::cerr << "Accept failed on serveur n " << servID << ": " << strerror(errno) << std::endl;
		return 0;
	}
	setNonBlockSocket(new_connexion);
	ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLERR | EPOLLHUP;
	ev.data.fd = new_connexion;
	std::cout << "j'ajoute new connexion qui est a " << new_connexion << std::endl;
	if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, new_connexion, &ev) == -1) {
		std::cerr << "Epoll ctl failed sur socket " << this->sockets[servID].listenFd << ": " << strerror(errno) << std::endl;
		return 0;
	}
	return 1;
}

int	Socket::setNonBlockSocket(int socket) {
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

int	Socket::isAnServerFd(int fd) {
	for (int i = 0; i < this->portListeningLen; i++) {
		if (this->sockets[i].listenFd == fd)
			return i;
	}
	return -1;
}