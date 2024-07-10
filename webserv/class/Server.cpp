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

static int	access_file(const std::string & confFile) {
	return access(confFile.c_str(), F_OK | R_OK);
}

static bool isOnlyWithSpace(std::string const & line)
{
	for (size_t i = 0; i < line.length(); i++) {
		if ((line[i] < 9 || line[i] > 13) && line[i] != 32)
			return false;
	}
	return true;
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
			v.push_back(line.substr(start, end - start));
		start = end + 1;
	}
	return v;
}

static void	handleServerParsing(std::vector<std::string> lineSplit) {
	(void)lineSplit;
	std::cout << "ServerPars" << std::endl;
}

static void	handleListenParsing(std::vector<std::string> lineSplit) {
	(void)lineSplit;
	std::cout << "ListenPars" << std::endl;
}

static void	handleServerNameParsing(std::vector<std::string> lineSplit) {
	(void)lineSplit;
	std::cout << "ServerNamePars" << std::endl;
}

static void	handleErrorPageParsing(std::vector<std::string> lineSplit) {
	(void)lineSplit;
	std::cout << "ErrorPageParse" << std::endl;
}

static void	handleClientMaxBodySizeParsing(std::vector<std::string> lineSplit) {
	(void)lineSplit;
	std::cout << "ClientMaxBodySizePars" << std::endl;
}

static void	handleLocationParsing(std::vector<std::string> lineSplit) {
	(void)lineSplit;
	std::cout << "LocationPars" << std::endl;
}

static bool AssignToken(std::vector<std::string> lineSplit, int countLine) {
	const std::string token[] = {"server", "listen", "server_name", "error_page"\
							, "client_max_body_size", "location"};
	void	(*FuncPtr[]) (std::vector<std::string>) = {&handleServerParsing, &handleListenParsing, &handleServerNameParsing\
		, &handleErrorPageParsing, &handleClientMaxBodySizeParsing, &handleLocationParsing};

	for (size_t i = 0; i < 6; i++) {
		if (*(lineSplit.begin()) == token[i])
			return FuncPtr[i](lineSplit), false;
	}
	std::cerr << "Invalid token [" << *(lineSplit.begin()) << "]" << " at line " << countLine << std::endl;
	return false;
}

static bool	ReadFile(const std::string & confFileFD) {
	std::string line;
	std::ifstream file(confFileFD.c_str());
	std::vector<std::string> lineSplit;

	int	line_count = 1;
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

	// faire une premiere boucle qui vas tout lire est verifier la syntaxe
	/* faire une deuxieme boucle et tout lire si la syntaxe est correct */
	while (getline(file, line))
	{
		if (line.empty() || isOnlyWithSpace(line))
			continue ;
		/* recuperer le premier mot de la ligne et le faire comp avec les mots
			cles et en fonctions de mots cles appliquer tels ou tel fonctions*/
		lineSplit = split(line);
		if (!AssignToken(lineSplit, line_count))
			return false;
		line_count++;
	}
	return true;
}

bool	Server::ft_parse_config_file(const std::string & confFile) {
	// ouvrir le fichier
	if (access_file(confFile) < 0)
		return false;
	// lire le fichier
	ReadFile(confFile);
	return true;
}