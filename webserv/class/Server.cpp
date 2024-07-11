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
			return FuncPtr[i](lineSplit), true;
	}
	std::cerr << "Invalid token [" << *(lineSplit.begin()) << "]" << " at line " << countLine << std::endl;
	return false;
}

// static void	printVector(std::vector<std::string> & v) {
// 	// std::vector<std::string>::iterator it = v.begin();
// 	std::vector<std::string>::iterator ite = v.end();

// 	std::cout << "last = " << *(--ite) << std::endl;
// 	// for (std::vector<std::string>::iterator it = v.begin(); it < v.end(); it++) {
// 	// 	std::cout << "v[] = " << *(it) << std::endl;
// 	// }
// }

bool	StrIsContext(std::string const & str) {
	const std::string context[] = {"server", "location"};
	for (size_t i = 0; i < str.size(); i++) {
		if (str == context[i])
			return true;
	}
	return false;
}

bool	StrSyntaxeCheck(std::string const & str) {
	return str[str.size() - 1] == ';' ? true : false;
}

static int	SyntaxParse(std::vector<std::string> & v, int countLine, int *OCB, int *CCB) {
	std::vector<std::string>::iterator ite = v.end();
	if (StrIsContext(*(v.begin())))
	{
		if (*(ite - 1) != "{")
		{
			std::cerr << "Error syntaxe: " << *(ite - 1) << " at line " << countLine \
				<< " curly brace is missing" << std::endl;
				return false;
		}
	}
	else
	{
		if (!StrSyntaxeCheck(*(ite - 1)))
		{
			std::cerr << "Error syntaxe: " << *(ite - 1) << " at line " << countLine \
				<< " ';' is missing at the end of line" << std::endl;
			return false;
		}
	}
	for (std::vector<std::string>::iterator it = v.begin(); it < v.end(); it++) {
		if (*(it) == "{}")
		{
			std::cerr << "Error syntaxe: " << *(it) << " at line " << countLine << std::endl;
			return false;
		}
		else if (*(it) == "{")
			(*OCB)++;
		else if (*(it) == "}")
			(*CCB)++;
		
	}
	return true;
}

static bool	ReadFile(const std::string & confFileFD) {
	std::string line;

	std::ifstream file(confFileFD.c_str());
	std::vector<std::string> lineSplit;

	int OCurlyBrace = 0;
	int CCurlyBrace = 0;
	int	countLine = 1;

	while (getline(file, line))
	{
		std::cout << "line = " << line << std::endl;
		if (line.empty() || isOnlyWithSpace(line))
			continue ;
		/* recuperer le premier mot de la ligne et le faire comp avec les mots
			cles et en fonctions de mots cles appliquer tels ou tel fonctions*/
		lineSplit = split(line);
		if (!SyntaxParse(lineSplit, countLine, &OCurlyBrace, &CCurlyBrace))
			return false;
		if (!AssignToken(lineSplit, countLine))
			return false;
		countLine++;
	}
	std::cout << OCurlyBrace << ' ' << CCurlyBrace << std::endl;
	if (OCurlyBrace != CCurlyBrace) {
		if (OCurlyBrace < CCurlyBrace)
			std::cerr << CCurlyBrace - OCurlyBrace << " Open curly brace is missing" << std::endl;
		else
			std::cerr << OCurlyBrace - CCurlyBrace << " Close curly brace is missing" << std::endl;
		return false;
	}
	return true;
}

bool	Server::ft_parse_config_file(const std::string & confFile) {
	// ouvrir le fichier
	if (access_file(confFile) < 0)
		return false;
	// lire le fichier
	if (!ReadFile(confFile))
		return false;
	return true;
}