#include "Server.hpp"

Server::Server() : _port(0),_socket(0), _serverName(0), _hostName(0) {}

Server::~Server() {}

uint16_t	Server::GetPort() const {
	return this->_port;
}

int	Server::GetSocket() const {
	return this->_socket;
}

const std::string Server::GetServerName() const {
	return this->_serverName;
}

const std::string Server::GetHostName() const {
	return this->_hostName;
}

struct sockaddr_in Server::GetAddr() const {
	return this->_addr;
}