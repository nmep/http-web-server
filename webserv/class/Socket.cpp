#include "Socket.hpp"

Socket::Socket()
{
	this->sockets = NULL;
}

// Socket::Socket(Socket const & copy) {}

Socket::~Socket()
{
	if (this->sockets)
		delete[] this->sockets;
}

// Socket& Socket::operator=(Socket const & rhs) {}

static bool	checkIfPortIsSet(int *SokcetPort, int value, int length) {
	std::cout << "value = " << value << " lengt = " << length << std::endl;
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
	int socketPort[conf.getNbServer()];
	int socketPortindex = 0;

	for (int i = 0; i < conf.getNbServer(); i++) {
		if (!checkIfPortIsSet(socketPort, conf.getServer(i).GetPort(), socketPortindex)) {
			socketPort[socketPortindex] = conf.getServer(i).GetPort();
			socketPortindex++;
		}
	}
	for (int i = 0; i < socketPortindex; i++) {
		std::cout << socketPort[i] << std::endl;
	}
	return 1;
}

int	Socket::initOneSocket(t_socket *socketStruct, Configuration const & conf, int index)
{
	// creation de socket

	socketStruct->socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketStruct->socketFd == -1) {
		std::cerr << "Error while creating socket " << strerror(errno) << std::endl;
		return 0;
	}
	socketStruct->addr.sin_family = AF_INET;
	socketStruct->addr.sin_addr.s_addr = INADDR_ANY;
	socketStruct->addr.sin_port = htons(conf.getServer(index).GetPort());

	std::memset(socketStruct->addr.sin_zero, '\0', sizeof(socketStruct->addr.sin_zero));

	// socket non bloquante
	// 	je recupere le flag de la socket pour l'utiliser ensuite
	socketStruct->socketFlag = fcntl(socketStruct->socketFd, F_GETFD, 0);
	if (socketStruct->socketFlag == -1) {
		std::cerr << "Error while creating non blocking socket " << strerror(errno) << std::endl;
		return 0;
	}

	if (fcntl(socketStruct->socketFd, F_SETFL, socketStruct->socketFlag | O_NONBLOCK) == -1) {
		std::cerr << "Error while creating non blocking socket " << strerror(errno) << std::endl;
		return 0;
	}

	int opt = 1;
	if (setsockopt(socketStruct->socketFd, SOL_SOCKET, SO_REUSEADDR, &opt , sizeof(int)) == -1) {
		std::cerr << "Error while set reuse socket: " << strerror(errno) << std::endl;
		return 0;
	}

	if (setsockopt(socketStruct->socketFd, SOL_SOCKET, SO_REUSEPORT, &opt , sizeof(int)) == -1) {
		std::cerr << "Error while set reuse socket: " << strerror(errno) << std::endl;
		return 0;
	}

	if (bind(socketStruct->socketFd, (const sockaddr *) &socketStruct->addr, sizeof(socketStruct->addr)) == -1) {
		std::cerr << "Error while binding socket " << strerror(errno) << std::endl;
		close (socketStruct->socketFd);
		return 0;
	}

	if (listen(socketStruct->socketFd, 2) == -1) { // deux connection max sont accepte s'il y en a plus alors elles seront refuse
		std::cerr << "Error while listening socket " << strerror(errno) << std::endl;
		close(socketStruct->socketFd);
		return 0;
	}
	return 1;
}