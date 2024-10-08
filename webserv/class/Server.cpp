#include "Server.hpp"

/* ----------------------------------------------------------------- */

Server::Server() : _default_server(0), _port(8080), _serverName("server_name"),\
	 _hostName("localhost"), _client_max_body_size(0), _autoIndex(false)
{
	std::cout << "def constructor autoindex = " << _autoIndex << std::endl;
	std::cout << BLUE << "Server default COnstructor called" << RESET << std::endl;
}

Server::~Server() {
	std::cout << BLUE << "Server destructor called" << RESET << std::endl;
	std::map<std::string, Location*>::iterator it = this->_location.begin();

	for (/**/; it != _location.end(); ++it) {
		if (it->second)
			delete it->second;
	}
	_location.clear();
}

Server::Server(Server const & copy)
{
	std::cout << "Server copy constructor called" << std::endl;
	*this = copy;
}

Server & Server::operator=(Server const & rhs)
{
	std::cout << "Server overload = constructor called" << std::endl;
	_default_server = rhs._default_server;
	_port = rhs._port;
	_serverName = rhs._serverName;
	_hostName = rhs._hostName;
	_error_page = rhs._error_page;
	_client_max_body_size = rhs._client_max_body_size;
	// check si loc de rhs est vide 
	if (rhs._location.size() > 0) {
		std::map<std::string, Location*>::const_iterator it_rhs = rhs._location.begin();
		std::map<std::string, Location*>::const_iterator ite_rhs = rhs._location.end();
		for (/**/; it_rhs != ite_rhs; it_rhs++) {
			this->_location[it_rhs->first] = new Location;
			*this->_location[it_rhs->first] = *it_rhs->second;
		}
	}
	return *this;
}

/* ----------------------------------------------------------------- */

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

std::string	Server::GetErrorPage(std::string const & httpCode) { // meme soucis que pour getlocation, si on envoie un truc qui existe pas encore ca le cree faut proteger to do
	return _error_page[httpCode];
}

uint16_t	Server::GetClientMaxBodySize(void) const {
	return _client_max_body_size;
}

std::string Server::GetHostName() const {
	return _hostName;
}

std::map<std::string, std::string> Server::getErrorPageMap() const
{
	return _error_page;
}

// return NULL si la location demande est inexistante
Location* Server::getLocation(std::string const & locationName)
{
	if (this->_location.size() == 0)
		return NULL;
	return this->_location[locationName];
}

std::map<std::string, Location*> Server::getLocationMap() const
{
	return _location;
}

bool Server::getAutoIndex() const {
	return _autoIndex;
}


/* ----------------------------------------------------------------- */

void	Server::SetDefaultServer()
{
	_default_server = true;
}

void	Server::SetPort(uint16_t & val) {
	_port = val;
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
				if (_error_page.find(*(lineSplit.begin() + i)) == _error_page.end())
					_error_page[*(lineSplit.begin() + i)] = *(lineSplit.end() - 1);
				else {
					std::cerr << "Invalid error page config: " << *(lineSplit.begin() + i) << " is already attribute tu an other error page, wich is " << GetErrorPage(*(lineSplit.begin() + i)) << " line: " << countLine << std::endl;
					return false;
				}
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
	_client_max_body_size = val;
}

void Server::SetHostName(std::string const & hostName) {
	_hostName = hostName;
}

/* --------------------------- PARSING -------------------------------------- */

bool	Server::isLocationExisting(std::string const & locationName) const
{
	if (_location.find(locationName) != _location.end())
		return true;
	return false;
}

bool	Server::handleListenParsing(std::vector<std::string> lineSplit, int countLine) {
	uint16_t port = 0;

	if (lineSplit.size() != 2) {
		std::cerr << "Invalid syntax: at line " << countLine << "should be listen	Port < 65535" << std::endl;
		return false;
	}
	// get the port without colom
	(lineSplit.begin() + 1)->erase((lineSplit.begin() + 1)->end() - 1);

	// case where there is no value in config fileFD after listen only a ';'
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
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid syntax: Server name need one value at line " << countLine << std::endl;
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

bool	Server::handleHostName(std::vector<std::string> lineSplit, int countLine)
{
	if (lineSplit.size() != 2) {
		std::cerr << "Error host name syntax: at line " << countLine << " must be only one value for the hostName" <<std::endl;
		std::cout << countLine << std::endl;
		return false;
	}
	// SetHostName(*(lineSplit.begin() + 1));
	return true;
}

void	Server::setAutoIndex(bool value) {
	_autoIndex = value;
}

/* --------------------------- PARSING -------------------------------------- */

bool	Server::handleAutoIndex(std::vector<std::string> lineSplit, int countLine) {
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid syntax: at line " << countLine << " Autoindex need a value (on or off)" << std::endl;
		return false;
	}

	// std::cout << "line begin = " << *(lineSplit.begin() + 1) << " avant" << std::endl;
	*(lineSplit.begin() + 1)->erase((lineSplit.begin() + 1)->end() - 1);
	// std::cout << "line begin = " << *(lineSplit.begin() + 1) << " apres" << std::endl;

	if (*(lineSplit.begin() + 1) != "on" && *(lineSplit.begin() + 1) != "off") {
		std::cerr << "Invalid AutoIndex Value at line " << countLine << " it must be on or off" << std::endl;
		return false;
	}

	std::cout << "je set autoindex dans serveur" << std::endl;
	if (*(lineSplit.begin() + 1) == "on")
		setAutoIndex(1);
	else
		setAutoIndex(0);
	return true;
}

bool Server::AssignToken(std::vector<std::string> lineSplit, int countLine) {
	const std::string fTokens[] = {"listen", "server_name", "error_page"\
							, "client_max_body_size", "autoindex"}; // pour location apelle directement getline dans
							// la fonction de location parse et voir si ca marche

	bool	(Server::*FuncPtr[]) (std::vector<std::string>, int) = {&Server::handleListenParsing, &Server::handleServerNameParsing\
		, &Server::handleErrorPageParsing, &Server::handleClientMaxBodySizeParsing, &Server::handleAutoIndex}; //TO DO manque  hostname

	// si une segment de directive et finit on return true et on passe au suivant
	if (*(lineSplit.begin()) == "}")
		return true;

	// si une directive est trouve faire sa fonction associe
	for (size_t i = 0; i < sizeof(fTokens) / sizeof(fTokens[0]); i++) {
		// std::cout << "ls = [" << *(lineSplit.begin()) << "]  ftoken[i] = " << fTokens[i] << std::endl;
		if (*(lineSplit.begin()) == fTokens[i]) {
			return (this->*FuncPtr[i])(lineSplit, countLine);
		}
	}
		std::cerr << "Invalid syntax: Server Invalid token [" << *(lineSplit.begin()) << ']' << " at line " << countLine  << std::endl;
	return false;
}

bool	Server::parseConfFile(std::ifstream & confFileFD, int *countLine) {
	std::string line;
	std::vector<std::string> lineSplit;

	while (getline(confFileFD, line))
	{
		if (line.empty() || isOnlyWithSpace(line) || isCommentary(line)) {
			(*countLine)++;
			continue ;
		}
		lineSplit = split(line);

		if (*(lineSplit.begin()) == "}")
			return true;

		else if (*(lineSplit.begin()) == "location") {
			if (lineSplit.size() != 3) {
				std::cerr << "Invalid Syntax: location need a match at line " << *countLine << std::endl;
				return false;
			}
			// Location &location = _location[*(lineSplit.begin() + 1)];
			(*countLine)++;
			_location[*(lineSplit.begin() + 1)] = new Location();
			// init autoindex de location de la meme valeur que celle d'autoindex de server
			if (this->_autoIndex)
				_location[*(lineSplit.begin() + 1)]->setAutoIndex(true);
			if (!_location[*(lineSplit.begin() + 1)]->LocationParsing(confFileFD, countLine))
				return false;
		}
		else if (!AssignToken(lineSplit, (*countLine)))
			return false;
		(*countLine)++;
	}
	return true;
}

std::ostream & operator<<(std::ostream & o, Server const & server)
{
    o << "SERVER PRINTING\n" << std::endl;
    o << "Default Server = " << server.GetDefaultServer() << std::endl;
    o << "Port = " << server.GetPort() << std::endl;
    o << "serverName = " << server.GetServerName() << std::endl;
    o << "hostName = " << server.GetHostName() << std::endl;
	o << "auto index = " << server.getAutoIndex() << std::endl;

    o << "Error page:" << std::endl;
	printMap(server.getErrorPageMap(), o);

    o << "Client max body size = " << server.GetClientMaxBodySize() << std::endl;

	o << "Location" << std::endl;
	if (server.getLocationMap().empty()) {
		o << "No location for this serv" << std::endl;
	}
	else {
		o << "il y a une location" << std::endl;
		std::map<std::string, Location*> map =  server.getLocationMap();
		std::map<std::string, Location*>::iterator it = map.begin();
		for (/**/; it != map.end(); it++) {
			o << "first = " << it->first << std::endl;
			o << "second = " << it->second << std::endl;
		}
	}
	return o;
}