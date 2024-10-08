#include "Socket.hpp"

Socket::Socket()
{
	this->portListening = NULL;
	this->sockets = NULL;
	this->portListeningLen = 0;
}

// Socket::Socket(Socket const & copy) {}

Socket::~Socket()
{
	if (this->sockets != NULL)
		delete[] this->sockets;
	if (this->portListening)
		delete[] this->portListening;
}

// Socket& Socket::operator=(Socket const & rhs) {}

static bool	checkIfPortIsSet(int *SokcetPort, int value, int length) {
	for (int i = 0; i < length; i++) {
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

	// remplir un tableau de port a mettre sur ecoute
	for (int i = 0; i < conf.getNbServer(); i++) {
		if (!checkIfPortIsSet(this->portListening, conf.getServer(i).GetPort(), this->portListeningLen)) {
			this->portListening[this->portListeningLen] = conf.getServer(i).GetPort();
			this->portListeningLen++;
		}
	}
	for (int i = 0; i < this->portListeningLen; i++) {
		std::cout << "dedans port = " << this->portListening[i] << std::endl;
	}
	// creer une socket pour chaque port
	for (int i = 0; i < this->portListeningLen; i++) {
		initOneSocket(&this->sockets[i], this->portListening[i]);
	}

	// debug
	for (int i = 0; i < this->portListeningLen; i++) {
		std::cout << "fd de port " << this->portListening[i] << " = " << this->sockets[i].listenFd << std::endl;
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
	socketStruct->socketFlag = fcntl(socketStruct->listenFd, F_GETFD, 0);
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