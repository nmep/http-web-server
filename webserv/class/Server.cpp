#include "Server.hpp"

Server::Server() : _port(0),_socket(0), _serverName(0), _hostName(0) {}

Server::~Server() {}

/* ----------------------------------------------------------------- */

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

int	Server::GetServerCount() const {
	return this->_server_count;
}

/* ----------------------------------------------------------------- */

void	Server::SetPort(uint16_t & val) {
	this->_port = val;
}

void	Server::SetSocket(int & val) {
	this->_socket = val;
}

void	Server::SetServerCount(int & val) {
	this->_server_count = val;
}

void	Server::SetServerName(std::string & val) {
	this->_serverName = val;
}

void Server::SetHostName(std::string & val) {
	this->_serverName = val;
}

void	Server::SetHostName(std::string & val) {
	this->_hostName = val;
}

void	Server::SetAddr(struct sockaddr_in & val) {
	this->_addr.sin_family = val.sin_family;
	this->_addr.sin_addr.s_addr = val.sin_addr.s_addr;
	this->_addr.sin_port = val.sin_port;
}

/* ----------------------------------------------------------------- */
