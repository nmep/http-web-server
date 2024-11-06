#include "Server.hpp"

/* ----------------------------------------------------------------- */

Server::Server() : _default_server(0), _serverName("server_name"),\
	 _hostName("localhost"), _client_max_body_size(1048576), _autoIndex(true), _isUploadFileAccepted(false), _serverIdx(0)
{
	//std::cout  << BLUE << "Server default COnstructor called" << RESET << std::endl;
	// //std::cout  << "size de vector port = " << this->_port.size() << std::endl;
}

/*

pour avoir le nom d'un dossier il faut que l'user l'entre dans le conf file

si il n'est pas entree je ne peux pas le changer dans le constructeur, c'est trop tot encore

si la conf est vide je peux 

*/

Server::~Server() {
	//std::cout  << BLUE << "Server destructor called" << RESET << std::endl;
	std::map<std::string, Location*>::iterator it = this->_location.begin();

	for (/**/; it != _location.end(); ++it) {
		if (it->second)
			delete it->second;
	}
	_location.clear();
}

Server::Server(Server const & copy) : _default_server(0), _port(8080), _serverName("server_name"),\
	 _hostName("localhost"), _client_max_body_size(0), _autoIndex(false), _isUploadFileAccepted(false)
{
	//std::cout  << "Server copy constructor called" << std::endl;
	*this = copy;
}

Server & Server::operator=(Server const & rhs)
{
	//std::cout  << "Server overload = constructor called" << std::endl;
	_default_server = rhs._default_server;
	_port = rhs._port;
	_serverName = rhs._serverName;
	_hostName = rhs._hostName;
	_error_page = rhs._error_page;
	_client_max_body_size = rhs._client_max_body_size;
	_isUploadFileAccepted = rhs._isUploadFileAccepted;
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

/*
Port est un vector
il cherche si val est dans le vector
	- si oui il le renvoit
	- si non il renvoit 0
*/

uint16_t	Server::GetPort(uint16_t val) const {
	 if (std::find(_port.begin(), _port.end(), val) != _port.end())
	 	return val;
	return 0;
}

std::vector<uint16_t>	&Server::GetPortVector() {
	return _port;
}

std::string Server::GetServerName() const {
	return _serverName;
}

// si httpCode n'est pas dans la map errorPage alors le getteur renvoie une empty string
std::string	Server::GetErrorPage(std::string const & httpCode) {
	if (this->_error_page.find(httpCode) != this->_error_page.end())
		return _error_page[httpCode];
	return std::string();
}

unsigned long long	Server::GetClientMaxBodySize(void) const {
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

std::string Server::getUploadStore() const {
	return _uploadStore;
}

bool Server::getIsUploadFileAccepted() const {
	return _isUploadFileAccepted;
}

int	Server::getServerIndex() const {
	return _serverIdx;
}

/* ----------------------------------------------------------------- */

void	Server::SetDefaultServer()
{
	_default_server = true;
}

void	Server::SetPort(uint16_t & val) {
	_port.push_back(val);
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

void	Server::SetClientMaxBodySize(unsigned long long & val) {
	_client_max_body_size = val;
}

void Server::SetHostName(std::string const & hostName) {
	_hostName = hostName;
}

void	Server::setUploadStore(std::string directoryUpload) {
	_uploadStore = directoryUpload;
}

bool	Server::isLocationExisting(std::string const & locationName) const
{
	if (_location.find(locationName) != _location.end())
		return true;
	return false;
}

void	Server::setIsUploadFileAccepted(bool value) {
	_isUploadFileAccepted = value;
}

void	Server::setServerIdx(int serverIdx) {
	_serverIdx = serverIdx;
}

/* --------------------------- PARSING -------------------------------------- */

bool	Server::handleListenParsing(std::vector<std::string>lineSplit, int countLine) {

	// get the port without colom
	(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->end() - 1);
	if ((lineSplit.end() - 1)->empty())
		lineSplit.erase(lineSplit.end() - 1);
	if (lineSplit.size() <= 1) {
		std::cerr << "Invalid syntax: at line " << countLine << "should be listen Port < 65535" << std::endl;
		return true;
	}

	for (std::vector<std::string>::iterator it = lineSplit.begin() + 1; it != lineSplit.end(); it++) {
		uint16_t port = 0;
		if (!strIsNum(*it)) {
			std::cerr << "Error Port settings : Listen Port " << *it << "at line " << countLine << std::endl;
			return false;
		}

		if (!ft_atoi_port(&port, *it)) {
			std::cerr << "Error Port settings : " << *it << " at line " << countLine << std::endl;
			return false;
		}
		SetPort(port);
	}
	return true;
}

bool	Server::handleServerNameParsing(std::vector<std::string> lineSplit, int countLine) {
	(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->end() - 1);
	if ((lineSplit.end() - 1)->empty()) {
		lineSplit.erase(lineSplit.end() - 1);
	}
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid syntax: Server name need one value at line " << countLine << std::endl;
		return true;
	}
	SetServerName(*(lineSplit.begin() + 1));
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
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid syntax: Client_Max_Body_size need to be: directive value " << countLine << std::endl;
		return false;
	}

	char type = (*(lineSplit.begin() + 1))[(lineSplit.begin() + 1)->size() - 2];
	// check si le type est bon
	if (type != 'M' && type != 'B' && type != 'K') {
		std::cerr << "Invalid syntax: Client_Max_Body_size content type need to be: B K or M " << countLine << std::endl;
		return false;
	}

	// delete type and ;
	(lineSplit.begin() + 1)->erase((lineSplit.begin() + 1)->size() - 2);
	// str is num till last
	for (size_t i = 0; i < (lineSplit.begin() + 1)->size(); i++) {
		if ((*(lineSplit.begin() + 1))[i] < '0' || (*(lineSplit.begin() + 1))[i] > '9') {
			std::cerr << "exept for the content type the max body size value most not contain non numeric character at line: " << countLine << std::endl;
			return false;
		}
	}

	unsigned long long val = 0;
	if (!ft_atoi_client_max_body_size(*(lineSplit.begin() + 1), &val)) {
		std::cerr << "Overflow detected on client max body size at line: " << countLine << std::endl;
		return false;
	}
	val = convert_bytes_into_type(val, type);
	if (!val) {
		std::cerr << "Error detected on client max body size at line while converting the value: " << countLine << std::endl;
		return false;
	}
	SetClientMaxBodySize(val);
	return true;
}

bool	Server::handleHostName(std::vector<std::string> lineSplit, int countLine)
{
	if (lineSplit.size() != 2) {
		std::cerr << "Error host name syntax: at line " << countLine << " must be only one value for the hostName" <<std::endl;
		//std::cout  << countLine << std::endl;
		return false;
	}
	SetHostName(*(lineSplit.begin() + 1));
	return true;
}

void	Server::setAutoIndex(bool value) {
	_autoIndex = value;
}

/* --------------------------- PARSING -------------------------------------- */

bool	Server::handleAutoIndex(std::vector<std::string> lineSplit, int countLine) {
	*(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->end() - 1);
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid syntax: at line " << countLine << " Autoindex need a value (on or off)" << std::endl;
		return false;
	}

	// //std::cout  << "line begin = " << *(lineSplit.begin() + 1) << " avant" << std::endl;
	// //std::cout  << "line begin = " << *(lineSplit.begin() + 1) << " apres" << std::endl;

	if (*(lineSplit.begin() + 1) != "on" && *(lineSplit.begin() + 1) != "off") {
		std::cerr << "Invalid AutoIndex Value at line " << countLine << " it must be on or off" << std::endl;
		return false;
	}

	if (*(lineSplit.begin() + 1) == "on")
		setAutoIndex(1);
	else
		setAutoIndex(0);
	return true;
}

bool Server::handleUploadStore(std::vector<std::string> lineSplit, int countLine)
{
	*(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->end() - 1);
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid upload_store syntax: no value associate, at line " << countLine << std::endl;
		return false;
	}
	// check if dir exist
	if (!checkAccessFile(*(lineSplit.begin() + 1), F_OK | R_OK | W_OK)) {
		std::cerr << "Invalid upload_store syntax: [" << *(lineSplit.begin() + 1) << "] " << strerror(errno) << " at line " << countLine << std::endl;
		return false;
	}
	if (!isDir(*(lineSplit.end() - 1))) {
		std::cerr << "Error upload store at line " << countLine << std::endl;
		return false;
	}
	setUploadStore(*(lineSplit.begin() + 1));
	setIsUploadFileAccepted(true);
	return true;
}

bool Server::AssignToken(std::vector<std::string> lineSplit, int countLine) {
	const std::string fTokens[] = {"listen", "server_name", "error_page"\
							, "client_max_body_size", "autoindex", "upload_store"}; // pour location apelle directement getline dans
							// la fonction de location parse et voir si ca marche

	bool	(Server::*FuncPtr[]) (std::vector<std::string>, int) = {&Server::handleListenParsing, &Server::handleServerNameParsing\
		, &Server::handleErrorPageParsing, &Server::handleClientMaxBodySizeParsing, &Server::handleAutoIndex, &Server::handleUploadStore}; //TO DO manque  hostname

	// si une segment de directive et finit on return true et on passe au suivant
	if (*(lineSplit.begin()) == "}")
		return true;

	// si une directive est trouve faire sa fonction associe
	for (size_t i = 0; i < sizeof(fTokens) / sizeof(fTokens[0]); i++) {
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
			(*countLine)++;
			if (!this->isLocationExisting(*(lineSplit.begin() + 1))) {
				_location[*(lineSplit.begin() + 1)] = new Location();
				if (this->_autoIndex)
					_location[*(lineSplit.begin() + 1)]->setAutoIndex(true);
			}
			if (!_location[*(lineSplit.begin() + 1)]->LocationParsing(confFileFD, countLine))
				return false;
		}
		else if (!AssignToken(lineSplit, (*countLine)))
			return false;
		(*countLine)++;
	}
	return true;
}

std::ostream & operator<<(std::ostream & o, Server & server)
{
    o << "SERVER PRINTING\n" << std::endl;
    o << "Default Server = " << server.GetDefaultServer() << std::endl;
    o << "Port = " << std::endl; printVector(server.GetPortVector(), o);
    o << "serverName = " << server.GetServerName() << std::endl;
    o << "hostName = " << server.GetHostName() << std::endl;
	o << "auto index = " << server.getAutoIndex() << std::endl;
	o << "IsUploadFileAccepted = " << server.getIsUploadFileAccepted() << std::endl;
	o << "upload store = " << server.getUploadStore() << std::endl;
	o << "Server Index = " << server.getServerIndex() << std::endl;

	o << "Error page:" << std::endl;
	printMap(server.getErrorPageMap(), o);

	o << "Client max body size = " << server.GetClientMaxBodySize() << std::endl;
	o << "Upload store = " << server.getUploadStore() << std::endl;
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

