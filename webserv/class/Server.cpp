#include "Server.hpp"

uint16_t					Server::_port = 0;
int							Server::_socket = 0;
int							Server::_server_count = 0;
std::string			Server::_serverName = "default";
std::string			Server::_hostName = "default";
struct sockaddr_in	Server::_addr;
bool						Server::_autoIndex = false;
Server::Server() {}

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
	Server::_port = val;
}

void	Server::SetSocket(int & val) {
	Server::_socket = val;
}

void	Server::SetServerCount(int & val) {
	Server::_server_count = val;
}

void	Server::SetServerName(std::string & val) {
	Server::_serverName = val;
}

void Server::SetHostName(std::string & val) {
	Server::_hostName = val;
}

void	Server::SetAddr(struct sockaddr_in & val) {
	Server::_addr.sin_family = val.sin_family;
	Server::_addr.sin_addr.s_addr = val.sin_addr.s_addr;
	Server::_addr.sin_port = val.sin_port;
}

void	Server::SetAutoIndex(int val) {
	Server::_autoIndex = val;
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

uint16_t	ft_atoi_port(uint16_t *ptr, std::string str) {
	int	i = 0;

	while ((str[i] > 9 && str[i] < 13) || str[i] == 32) {
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9') {
		if (*ptr > PORT_MAX / 10 || (*ptr > PORT_MAX / 10 && (str[i] - 48) > PORT_MAX % 10))
			return false;
		*ptr = *ptr * 10 + (str[i] - 48);
		i++;
	}
	return true;
}

bool	strIsNum(std::string str) {
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] < '0' || str[i] > '9') {
			return false;
		}
	}
	return true;
}

bool	handleListenParsing(std::vector<std::string> lineSplit, int countLine) {
	uint16_t port = 0;

	if (lineSplit.size() != 2) {
		std::cerr << "Invalid syntax: at line " << countLine << "should be listen	Port < 65535" << std::endl;
		return false;
	}
	// get the port without colom
	std::string portWithoutColom = (lineSplit.begin() + 1)->erase((lineSplit.begin() + 1)->size() - 1);

	// case where there is no value in config file after listen only a ';'
	if (portWithoutColom == "") {
		std::cerr << "Invalid syntax: Port value is invalid at line " << countLine << std::endl;
		return false;
	}

	if (!strIsNum((*(lineSplit.begin() + 1)))) {
		std::cerr << "Invalid syntax: Listen Port " << *(lineSplit.begin() + 1) << "at line " << countLine << std::endl;
		return false;
	}

	if (!ft_atoi_port(&port, *(lineSplit.begin() + 1))) {
		std::cerr << "Invalid syntax " << *(lineSplit.begin() + 1) << " at line " << countLine << std::endl;
		return false;
	}

	Server::SetPort(port);
	return true;
}

bool	handleServerNameParsing(std::vector<std::string> lineSplit, int countLine) {
	(void)lineSplit;
	(void)countLine;
	std::cout << "ServerNamePars" << std::endl;
	return true;
}

bool	handleErrorPageParsing(std::vector<std::string> lineSplit, int countLine) {
	(void)lineSplit;
	(void)countLine;
	std::cout << "ErrorPageParse" << std::endl;
	return true;
}

bool	handleClientMaxBodySizeParsing(std::vector<std::string> lineSplit, int countLine) {
	(void)lineSplit;
	(void)countLine;
	std::cout << "ClientMaxBodySizePars" << std::endl;
	return true;
}

bool	handleLocationParsing(std::vector<std::string> lineSplit, int countLine) {
	(void)lineSplit;
	(void)countLine;
	std::cout << "LocationPars" << std::endl;
	return true;
}

bool AssignToken(std::vector<std::string> lineSplit, int countLine) {
	const std::string fTokens[] = {"listen", "server_name", "error_page"\
							, "client_max_body_size", "location"}; // pour location apelle directement getline dans
							// la fonction de location parse et voir si ca marche
	const std::string cTokens[] = {"server", "listen", "server_name", "error_page", "client_max_body_size", "location", "}"};
	bool	(*FuncPtr[]) (std::vector<std::string>, int) = {&handleListenParsing, &handleServerNameParsing\
		, &handleErrorPageParsing, &handleClientMaxBodySizeParsing, &handleLocationParsing};

	if (*(lineSplit.begin()) == "}")
		return true;

	for (size_t i = 0; i < 5; i++) {
		if (*(lineSplit.begin()) == fTokens[i])
			return FuncPtr[i](lineSplit, countLine);
	}

	for (size_t i = 0; i <= cTokens->size(); i++) {
		if (*(lineSplit.begin()) == cTokens[i])
			return true; // break 

		if (i == cTokens->size()) {
			std::cerr << "Invalid syntax: Invalid token [" << *(lineSplit.begin()) << "] at line " << countLine << std::endl;
			return false;
		}
	}

	std::cerr << "Invalid syntax: " << *(lineSplit.begin()) << " at line " << countLine << std::endl;
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
	const std::string context[] = {"server", "location", "}"};

	for (size_t i = 0; context[i] != "\0" ; i++) {
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
		if (*(ite - 1) != "{" && *(ite - 1) != "}")
		{
			std::cerr << "Invalid syntax: " << *(ite - 1) << " at line " << countLine \
				<< " curly brace is missing" << std::endl;
				return false;
		}
	}
	else
	{
		if (!StrSyntaxeCheck(*(ite - 1)))
		{
			std::cerr << "Invalid syntax: " << *(ite - 1) << " at line " << countLine \
				<< " ';' is missing at the end of line" << std::endl;
			return false;
		}
	}
	for (std::vector<std::string>::iterator it = v.begin(); it < v.end(); it++) {
		if (*(it) == "{}")
		{
			std::cerr << "Invalid syntax: " << *(it) << " at line " << countLine << std::endl;
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
		if (line.empty() || isOnlyWithSpace(line))
			continue ;
		/* recuperer le premier mot de la ligne et le faire comp avec les mots
			cles et en fonctions de mots cles appliquer tels ou tel fonctions*/
		lineSplit = split(line);
		// check si la ligne est en dehors du scope des accolade, si oui mettre faux
		if ((OCurlyBrace == CCurlyBrace && OCurlyBrace > 0) && *(lineSplit.begin()) != "server") {
			std::cerr << "Invalid syntax: element -> " << line << " isn't in the scope at the line " << countLine << std::endl;   
			return false;
		}
		// check si le premier mot est correct
		if (!SyntaxParse(lineSplit, countLine, &OCurlyBrace, &CCurlyBrace))
			return false;
		if (!AssignToken(lineSplit, countLine))
			return false;
		countLine++;
	}

	std::cout << OCurlyBrace << ' ' << CCurlyBrace << std::endl;
	if (OCurlyBrace != CCurlyBrace) {
		if (OCurlyBrace < CCurlyBrace)
			std::cerr << "Invalid syntax: " << CCurlyBrace - OCurlyBrace << " Open curly brace is missing" << std::endl;
		else
			std::cerr << "Invalid syntax: " << OCurlyBrace - CCurlyBrace << " Close curly brace is missing" << std::endl;
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