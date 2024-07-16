#include "Server.hpp"

uint16_t							Server::_port = 0;
int									Server::_socket = 0;
int									Server::_server_count = 0;
std::vector<std::string>			Server::_serverName;
std::map<std::string, std::string>	Server::_error_page;
uint16_t								Server::_client_max_body_size;
std::string							Server::_hostName = "default";
struct sockaddr_in					Server::_addr;
bool								Server::_autoIndex = false;
Server::Server() {}

Server::~Server() {}

/* ----------------------------------------------------------------- */

bool	strIsNum(std::string str);
bool	CheckErrorClientValue(std::string & str);

uint16_t	Server::GetPort() const {
	return this->_port;
}

int	Server::GetSocket() const {
	return this->_socket;
}

std::string Server::GetServerName(int index) const {
	if (index <= 0) {
		index = 1;
	}
	else if (index > (int) Server::_serverName.size()) {
		index = Server::_serverName.size();
	}
	return Server::_serverName[index - 1];
}

std::string	Server::GetErrorPage(std::string httpCode) {
	if (Server::_error_page[httpCode].size() > 0)
		return Server::_error_page[httpCode];
	return "";
}

uint16_t	Server::GetClientMaxBodySize(void) {
	return Server::_client_max_body_size;
}

std::string Server::GetHostName() const {
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
	Server::_serverName.push_back(val);
}

void	printMap(std::map<std::string, std::string> map) {
	std::map<std::string, std::string>::iterator it = map.begin();
	std::map<std::string, std::string>::iterator ite = map.end();

	for (/**/; it != ite; it++) {
		std::cout << "map first = " << it->first << " map second = " << it->second << std::endl;
	}
}

bool	Server::SetErrorPage(std::vector<std::string> lineSplit, int countLine) {
	// std::map<std::string, std::string>::iterator it = Server::_error_page.begin();
	// std::map<std::string, std::string>::iterator ite = Server::_error_page.end();

	for (size_t i = 1; i < lineSplit.size() - 1; i++) {
		if (strIsNum(*(lineSplit.begin() + i))) {
			if (!CheckErrorClientValue(*(lineSplit.begin() + i))) {
				std::cerr << "Invalid syntax: HTPP code " << *(lineSplit.begin() + i) << " is invalid at line " << countLine << std::endl;
				return false;
			}
			else {
				Server::_error_page[*(lineSplit.begin() + i)] = *(lineSplit.end() - 1);
			}
		}
		else {
			std::cerr << "Invalid syntax: " << "HTPP port " << *(lineSplit.begin() + i) << " is invalid" << std::endl;
			return false;
		}
	}
	return true;
}

void	Server::SetClientMaxBodySize(uint16_t & val) {
	Server::_client_max_body_size = val;
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

// static void	printVector(std::vector<std::string> & v) {
// 	// std::vector<std::string>::iterator it = v.begin();
// 	std::vector<std::string>::iterator ite = v.end();

// 	std::cout << "last = " << *(--ite) << std::endl;
// 	for (std::vector<std::string>::iterator it = v.begin(); it < v.end(); it++) {
// 		std::cout << "v[] = " << *(it) << std::endl;
// 	}
// }

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

// rajouter un serverparse pour compter le nombre de serveur ?

bool	handleListenParsing(std::vector<std::string> lineSplit, int countLine) {
	uint16_t port = 0;
 
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid syntax: at line " << countLine << "should be listen	Port < 65535" << std::endl;
		return false;
	}
	// get the port without colom
	(lineSplit.begin() + 1)->erase((lineSplit.begin() + 1)->end() - 1);

	// case where there is no value in config file after listen only a ';'
	if (*(lineSplit.begin() + 1) == "") {
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
	if (lineSplit.size() <= 1) {
		std::cerr << "Invalid syntax: Server name need content at line " << countLine << std::endl;
		return false;
	}
	for (size_t i = 1; i < lineSplit.size(); i++) {
		if (i == lineSplit.size() - 1) {
			(lineSplit.begin() + i)->erase((lineSplit.begin() + i)->end() - 1);
		}
		Server::SetServerName(*(lineSplit.begin() + i));
	}
	return true;
}

bool	CheckErrorClientValue(std::string & str) {
	if ((str >= "400" && str <= "419") || (str >= "421" && str <= "429") || str == "431"
		|| (str >= "449" && str <= "451") || str == "456") {
			return true;
	}
	return false;
}

bool	handleErrorPageParsing(std::vector<std::string> lineSplit, int countLine) {
	if (lineSplit.size() == 1) {
		std::cerr << "Invalid syntax: Errorpage need content at line " << countLine << std::endl;
		return false;
	}
	Server::SetErrorPage(lineSplit, countLine);
	return true;
}

bool	handleClientMaxBodySizeParsing(std::vector<std::string> lineSplit, int countLine) {
	if (lineSplit.size() <= 1) {
		std::cerr << "Invalid syntax: Client_Max_Body_size need content at line " << countLine << std::endl;
		return false;
	}

	uint16_t cmbs;
	std::cout << "test de atoi sur " << *(lineSplit.begin() + 1) << std::endl;
	int MPos = (lineSplit.begin() + 1)->find("M");
	if (MPos <= 0) {
		std::cerr << "Invalid syntax: Client max body size value [" << *(lineSplit.begin() + 1) << "] Need a value or a type of data (M)" << std::endl;
		return false;
	}
	std::string clientmaxbodysize = (lineSplit.begin() + 1)->substr(0, MPos);

	if (strIsNum(clientmaxbodysize)) {	
		ft_atoi_port(&cmbs, clientmaxbodysize);
		if (cmbs > 0 && cmbs <= 200) {
			Server::SetClientMaxBodySize(cmbs);
		}
		else {
			std::cerr << "Invalid Syntax: Max body client size " << cmbs << " is to large max 200" << std::endl;
			return false;
		}
	}
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
			return true;

		if (i == cTokens->size()) {
			std::cerr << "Invalid syntax: Invalid token [" << *(lineSplit.begin()) << "] at line " << countLine << std::endl;
			return false;
		}
	}

	std::cerr << "Invalid syntax: " << *(lineSplit.begin()) << " at line " << countLine << std::endl;
	return false;
}

bool	StrIsContext(std::string const & str) {
	const std::string context[] = {"server", "location", "}"};

	for (size_t i = 0; i < sizeof(context) / sizeof(std::string) ; i++) {
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