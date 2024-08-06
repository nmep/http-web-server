#include "Server.hpp"

/* ----------------------------------------------------------------- */

Server::Server() : _default_server(0), _port(8080), _serverName("server_name"), _hostName("localhost"), _autoIndex(false) {}

Server::~Server() {}

/* ----------------------------------------------------------------- */

bool	strIsNum(std::string str);
bool	CheckErrorClientValue(std::string & str);
bool	checkHtmlAccess(std::string filePath);

bool	Server::GetDefaultServer() const
{
	return _default_server;
}

uint16_t	Server::GetPort() const {
	return _port;
}

std::string Server::GetServerName() const {
	return _serverName;
}

std::string	Server::GetErrorPage(std::string const & httpCode) {
	return Server::_error_page[httpCode];
}

uint16_t	Server::GetClientMaxBodySize(void) {
	return _client_max_body_size;
}

std::string Server::GetHostName() const {
	return _hostName;
}

bool Server::GetAutoIndex() const {
	return _autoIndex;
}

/* ----------------------------------------------------------------- */

void	Server::SetDefaultServer()
{
	_default_server = true;
}

void	Server::SetPort(uint16_t & val) {
	Server::_port = val;
}

void	Server::SetServerName(std::string const & serverName) {
	_serverName = serverName;
}

bool	Server::SetErrorPage(std::vector<std::string> lineSplit, int countLine) {
	// cut le colom du last
	(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->size() - 1);
	// check access
	if (!checkHtmlAccess(*(lineSplit.end() - 1))) {
		return false;
	}
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

void Server::SetHostName(std::string const & hostName) {
	Server::_hostName = hostName;
}

void	Server::SetAutoIndex(int val) {
	Server::_autoIndex = val;
}

/* --------------------------- PARSING -------------------------------------- */

void	printVector(std::vector<std::string> v) {
	for (std::vector<std::string>::iterator it = v.begin(); it < v.end(); it++) {
		std::cout << "v[] = " << *(it) << std::endl;
	}
}

void	printLocation(std::map<std::string, std::map<std::string, std::vector<std::string> > > map) {
	std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator it1 = map.begin();
	std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator ite1 = map.end();

	std::map<std::string, std::vector<std::string> >::iterator it2;
	std::map<std::string, std::vector<std::string> >::iterator ite2;
	for (/**/; it1 != ite1; it1++) {
		std::cout << "first MAP = " << (*it1).first << std::endl;

		it2 = it1->second.begin();
		ite2 = it1->second.end();

		for (/**/; it2 != ite2; it2++) {
			std::cout << "location directive = " << (*it2).first << std::endl;
			std::cout << "location directive value = " << std::endl;
			printVector(it2->second);
		}
		
	}
}

bool isOnlyWithSpace(std::string const & line)
{
	for (size_t i = 0; i < line.length(); i++) {
		if ((line[i] < 9 || line[i] > 13) && line[i] != 32)
			return false;
	}
	return true;
}

std::vector<std::string>	split(std::string & line) {
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
	uint16_t	i = 0;

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

bool	CheckErrorClientValue(std::string & str) {
	if ((str >= "400" && str <= "419") || (str >= "421" && str <= "429") || str == "431"
		|| (str >= "449" && str <= "451") || str == "456") {
			return true;
	}
	return false;
}

bool	checkHtmlAccess(std::string filePath) {
	if (access(filePath.c_str(), F_OK | R_OK) < 0) {
		std::cerr << "Invalid syntax: " << "Access on html file [" << filePath << "]: " << strerror(errno) << std::endl;
		return false;
	}
	return true;
}

bool	Server::handleListenParsing(std::vector<std::string> lineSplit, int countLine) {
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
	SetPort(port);
	return true;
}

bool	Server::handleServerNameParsing(std::vector<std::string> lineSplit, int countLine) {
	if (lineSplit.size() <= 1) {
		std::cerr << "Invalid syntax: Server name need content at line " << countLine << std::endl;
		return false;
	}
	for (size_t i = 1; i < lineSplit.size(); i++) {
		if (i == lineSplit.size() - 1) {
			(lineSplit.begin() + i)->erase((lineSplit.begin() + i)->end() - 1);
		}
		SetServerName(*(lineSplit.begin() + i));
	}
	return true;
}

bool	Server::handleErrorPageParsing(std::vector<std::string> lineSplit, int countLine) {
	// check avec access le fichier html
	if (lineSplit.size() == 1) {
		std::cerr << "Invalid syntax: Errorpage need content at line " << countLine << std::endl;
		return false;
	}

	if (!SetErrorPage(lineSplit, countLine))
		return false;
	return true;
}

bool	Server::handleAutoIndex(std::vector<std::string> lineSplit, int countLine) {
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid Syntax: at line " << countLine << " Autoindex need a value (on or off)" << std::endl;
		return false;
	}
	std::cout << "line begin = " << *(lineSplit.begin() + 1) << " avant" << std::endl;
	*(lineSplit.begin() + 1)->erase((lineSplit.begin() + 1)->end() - 1);
	std::cout << "line begin = " << *(lineSplit.begin() + 1) << " apres" << std::endl;

	if (*(lineSplit.begin() + 1) != "on" && *(lineSplit.begin() + 1) != "off") {
		std::cerr << "Invalid AutoIndex Value at line " << countLine << " it must be on or off" << std::endl;
		return false;
	}
	if (*(lineSplit.begin() + 1) == "on")
		SetAutoIndex(1);

	else
		SetAutoIndex(0);
	return true;
}

bool	Server::handleClientMaxBodySizeParsing(std::vector<std::string> lineSplit, int countLine) {
	if (lineSplit.size() <= 1) {
		std::cerr << "Invalid syntax: Client_Max_Body_size need content at line " << countLine << std::endl;
		return false;
	}

	uint16_t cmbs = 0;
	int MPos = (lineSplit.begin() + 1)->find("M");
	if (MPos <= 0) {
		std::cerr << "Invalid syntax: Client max body size value [" << *(lineSplit.begin() + 1) << "] Need a value or a type of data (M)" << std::endl;
		return false;
	}

	std::string clientmaxbodysize = (lineSplit.begin() + 1)->substr(0, MPos);

	if (strIsNum(clientmaxbodysize)) {	
		ft_atoi_port(&cmbs, clientmaxbodysize);
		if (cmbs > 0 && cmbs <= 200) {
			SetClientMaxBodySize(cmbs);
		}
		else {
			std::cerr << "Invalid Syntax: Max body client size " << cmbs << " is to large max 200" << std::endl;
			return false;
		}
	}
	return true;
}

bool	isAllowedMethodsValid(std::vector<std::string> allowedMethods, int countLine) {
	std::string	AcceptMethods[] = {"GET", "POST"};
	std::vector<std::string>::iterator it = allowedMethods.begin() + 1;

	if (allowedMethods.size())
	for (/*nope*/; it != allowedMethods.end(); it++) {
		
		for (size_t i = 0; i < AcceptMethods->size() - 1; i++) {
			if (*it == AcceptMethods[i])
				break ;
			else if (i + 1 == AcceptMethods->size() - 1) {
				std::cerr << "Invalid Allowed methods: Line " << countLine << " [" << *it << ']' << std::endl;
				return false;
			}
		}
	}
	return true;
}

bool Server::AssignToken(std::vector<std::string> lineSplit, int countLine) {
	const std::string fTokens[] = {"listen", "server_name", "error_page"\
							, "client_max_body_size", "autoindex"}; // pour location apelle directement getline dans
							// la fonction de location parse et voir si ca marche
	const std::string cTokens[] = {"server", "listen", "server_name", "error_page", "client_max_body_size", "location", "}"};
	bool	(Server::*FuncPtr[]) (std::vector<std::string>, int) = {&Server::handleListenParsing, &Server::handleServerNameParsing\
		, &Server::handleErrorPageParsing, &Server::handleClientMaxBodySizeParsing, &Server::handleAutoIndex};

	// si une segment de directive et finit on return true et on passe au suivant
	if (*(lineSplit.begin()) == "}")
		return true;

	// si une directive est trouve faire sa fonction associe
	for (size_t i = 0; i < 5; i++) {
		if (*(lineSplit.begin()) == fTokens[i]) {
			return (this->*FuncPtr[i])(lineSplit, countLine);
		}
	}

	// verifier si le mot est valide
	for (size_t i = 0; i <= cTokens->size(); i++) {
		if (*(lineSplit.begin()) == cTokens[i])
			return true;
		// if (i == cTokens->size()) {
		// 	std::cerr << "Invalid syntax: Invalid token [" << *(lineSplit.begin()) << "] at line " << countLine << std::endl;
		// 	return false; C'EST UTILE CA ??
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

bool	Server::ReadFile(const std::string & confFileFD) {
	std::string line;

	std::ifstream file(confFileFD.c_str());
	std::vector<std::string> lineSplit;

	// int OCurlyBrace = 0;
	// int CCurlyBrace = 0;
	int	countLine = 1;

	while (getline(file, line))
	{
		if (line.empty() || isOnlyWithSpace(line)) {
			countLine++;
			continue ;
		}
		/* recuperer le premier mot de la ligne et le faire comp avec les mots
			cles et en fonctions de mots cles appliquer tels ou tel fonctions*/
		lineSplit = split(line);

		// check si la ligne est en dehors du scope des accolade, si oui mettre faux
		// if ((OCurlyBrace == CCurlyBrace && OCurlyBrace > 0) && *(lineSplit.begin()) != "server") {
		// 	std::cerr << "Invalid syntax: element -> " << line << " isn't in the scope at the line " << countLine << std::endl;   
		// 	return false;
		// }

		// check si le premier mot est correct
		// if (!SyntaxParse(lineSplit, countLine, &OCurlyBrace, &CCurlyBrace))
		// 	return false;

		// location est traite differement des autres parce qu'il a besoin de plus de parametres
		// if (*(lineSplit.begin()) == "location") {
		// 	if (!handleLocationParsing(lineSplit, &countLine, &OCurlyBrace, &CCurlyBrace, file, line)) {
		// 		countLine++;
		// 		return false;
		// 	}
		// }

		if (!AssignToken(lineSplit, countLine))
			return false;
		countLine++;
	}

	// if (OCurlyBrace != CCurlyBrace) {
	// 	if (OCurlyBrace < CCurlyBrace)
	// 		std::cerr << "Invalid syntax: " << CCurlyBrace - OCurlyBrace << " Open curly brace is missing" << std::endl;
	// 	else
	// 		std::cerr << "Invalid syntax: " << OCurlyBrace - CCurlyBrace << " Close curly brace is missing" << std::endl;
	// 	return false;
	// }
	return true;
}

// bool	Server::ft_parse_config_file(const std::string & confFile) {
	// ouvrir le fichier
	// if (access_file(confFile) < 0)
		// return false;
	// lire le fichier
	// if (!ReadFile(confFile))
	// 	return false;
// 	return true;
// }