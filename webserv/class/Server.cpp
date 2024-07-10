#include "Server.hpp"

Server::Server() : _port(0), _socket(0), _server_count(0), \
	_serverName("default"), _hostName("default") {}

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

bool Server::GetAutoIndex() const {
	return this->_autoIndex;
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

void	Server::SetAddr(struct sockaddr_in & val) {
	this->_addr.sin_family = val.sin_family;
	this->_addr.sin_addr.s_addr = val.sin_addr.s_addr;
	this->_addr.sin_port = val.sin_port;
}

void	Server::SetAutoIndex(int val) {
	this->_autoIndex = val;
}

/* --------------------------- PARSING -------------------------------------- */

static int	ft_access_file(const std::string & confFile) {
	return access(confFile.c_str(), F_OK | R_OK);
}

static std::vector<std::string>	split(std::string & line) {
	std::vector<std::string> v;
	size_t start = 0;
	size_t end = 0;

	while (end != std::string::npos)
	{
		if (line.find(9, start) < line.find(32, start))
			end = line.find(9, start);
		else
			end = line.find(32, start);
		if (end > start)
		{
			v.push_back(line.substr(start, end - start));
			std::cout << "v = [" << v.back() << "]" << std::endl;
		}
		start = end + 1;
	}
	return v;
}

static void	ReadFile(const std::string & confFileFD) {
	std::string line;
	std::ifstream file(confFileFD.c_str());
	// int	line_count = 0;
	// server
		/* conte les braqui ouvrante et fermante */
	// listen
		// split et recuperer la valeur du port
	// server_name
		// split aussi
	// error_page
		// split aussi
	// client_max_body_size
		// split
	// location


	/* faire un boucle de read  */
	while (getline(file, line))
	{
		/* recuperer le premier mot de la ligne et le faire comp avec les mots
			cles et en fonctions de mots cles appliquer tels ou tel fonctions*/
		split(line);

	}
}


void	Server::ft_parse_config_file(const std::string & confFile) {
	// ouvrir le fichier
	if (ft_access_file(confFile) < 0)
		throw std::invalid_argument("Config file error: access");
	// lire le fichier
	ReadFile(confFile);
}