#include "Location.hpp"

int Location::_locationIndex = 0;

Location::Location() : _autoIndex(false)
{
	_locationIndex++;
	_locationID = _locationIndex;
	_allowedMethod = std::vector<std::string>(1);
	std::cout << GREEN << "location default constructor de location id " << _locationIndex << " called" << RESET << std::endl;
}

Location::Location(Location const & copy) :_autoIndex(false)
{
	std::cout << "j'incremente location index" << std::endl;
	_locationIndex++;
	_locationID = _locationIndex;
	*this = copy;
}

Location & Location::operator=(Location const & rhs)
{
	if (rhs._allowedMethod.size()) {
		_allowedMethod.insert(_allowedMethod.begin(), rhs._allowedMethod.begin(), rhs._allowedMethod.end());
		_allowedMethod.resize(rhs._allowedMethod.size());
	}
	if (!rhs._redirection->empty()) {
		_redirection[0] = rhs._redirection[0];
	}
		_redirection[1] = rhs._redirection[1];
	_root = rhs._root;
	_autoIndex = rhs._autoIndex;
	_index = rhs._index;
	return *this;
}

Location::~Location() {
	std::cout << GREEN << "Location destructeur de location id " << _locationID << " called" << RESET << std::endl;
}

bool Location::getAllowedMethod(std::string const & method) const {
	std::vector<std::string>::const_iterator it = _allowedMethod.begin();
	for (/**/; it != _allowedMethod.end(); it++) {
		if (*it == method)
			return true;
	}
	return false;
}

std::string Location::getRedirection(std::string const & returnValue) const {
	if (!_redirection)
		return "";
	if (returnValue == "CODE")
		return _redirection[0];
	return _redirection[1];
}

std::string Location::getRoot() const {
	return _root;
}

bool Location::getAutoIndex() const {
	return _autoIndex;
}

int		Location::getLocationID() const {
	return _locationID;
}

// renvoie une vector empty si allowedMethods est vide
std::vector<std::string> Location::getAllowedMethodVector() const {
	std::vector<std::string> empty;

	if (this->_allowedMethod.empty()) {
		std::cout << "allowed est vide je renvoie un vector empty" << std::endl;
		return empty;
	}
	return _allowedMethod;
}

// renvoie une vector empty si index est vide
// pour eviter tout segfault, sil vous plait utiliser d'abord getlocation pour etre sur que ca existe
std::vector<std::string> Location::getIndex() const {
	std::vector<std::string> empty;

	if (this->_index.empty()) {
		std::cout << "index est vide je renvoie un vector empty" << std::endl;
		return empty;
	}
	return _index;
}

std::vector<std::pair<std::string, std::string> > & Location::getPairCgi() {
	return _cgi;
}

/* -------------------------------------------------------------------- */

void	Location::setAllowedMethod(std::vector<std::string> const & allowedMethod) {
	this->_allowedMethod = allowedMethod;
}

void	Location::setRedirection(std::vector<std::string> const & redirection) {
	_redirection[0] = redirection[0];
	_redirection[1] = redirection[1];
}

void	Location::setRoot(std::string const & value) {
	_root = value;
}

void	Location::setAutoIndex(bool value) {
	_autoIndex = value;
}

void	Location::setIndex(std::string const & indexFileName) {
	_index.push_back(indexFileName);
}

void	Location::setCgi(std::string mime, std::string cgiPath) {
	this->_cgi.push_back(std::make_pair(mime, cgiPath));
}

/* -------------------------------------------------------------------- */

// renvoie true si le mime est dans la conf et false dans le cas inverse
bool	Location::isCgiMimeAccepted(std::string cgiMime) const {
	for (std::vector<std::pair<std::string, std::string> >::const_iterator it =_cgi.begin(); it != _cgi.end(); it ++) {
		if (it->first == cgiMime)
			return true;
	}
	return false;
}

// renvoie true si le path est dans la conf et false dans le cas inverse
bool	Location::isCgiPathAccepted(std::string path) const {
	for (std::vector<std::pair<std::string, std::string> >::const_iterator it =_cgi.begin(); it != _cgi.end(); it ++) {
		if (it->second == path)
			return true;
	}
	return false;
}

/* -------------------------------------------------------------------- */

bool	Location::handleAutoIndex(std::vector<std::string> lineSplit, int countLine) {
	*(lineSplit.begin() + 1)->erase((lineSplit.begin() + 1)->end() - 1);
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid syntax: at line " << countLine << " Autoindex need a value (on or off)" << std::endl;
		return false;
	}

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

bool	Location::handleRoot(std::vector<std::string> lineSplit, int countLine)
{
	(lineSplit.end() - 1)->erase((lineSplit.end() -1)->end() - 1);
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid Root syntax it must be root <root_path>, at line " << countLine << std::endl;
		return false;
	}
	if (!checkAccessFile(*(lineSplit.begin() + 1), F_OK | R_OK | W_OK)) {
		std::cerr << "Error Location parsing: Root " << *(lineSplit.begin() + 1) << " set a line " << countLine << ": " << strerror(errno) << std::endl;
		return false;
	}
	if (!isDir(*(lineSplit.end() - 1)))
		return false;
	setRoot(*(lineSplit.begin() + 1));
	return true;
}

bool	Location::handleIndex(std::vector<std::string> lineSplit, int countLine)
{
	if (lineSplit.size() == 1) {
		std::cerr << "Invalid index syntax it must be index <index_path>, at line " << countLine << std::endl;
		return false;
	}
	(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->end() - 1);
	for (std::vector<std::string>::iterator it = lineSplit.begin() + 1; it != lineSplit.end(); it++) {
		setIndex(*it);
	}
	return true;
}

bool	Location::handleAllowedMethods(std::vector<std::string> lineSplit, int countLine)
{
	if (lineSplit.size() == 1) {
		std::cerr << "Invalid Allowed methods syntax: no value associate, at line " << countLine << std::endl;
		return false;
	}
	// erase le point virgule
	(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->end() - 1);
	for (std::vector<std::string>::iterator it = (lineSplit.begin() + 1); it != lineSplit.end(); ++it) {
		if (*it != "GET" && *it != "POST" && *it != "DELETE") {
			std::cerr << "Invalid Allowed methods syntax: [" << *it << "] isn't accepted it must be GET POST or DELETE, at line " << countLine << std::endl;
			return false;
		}
	}
	// erase le premier mot
	lineSplit.erase(lineSplit.begin());
	setAllowedMethod(lineSplit);
	return true;
}

bool	Location::handleRedirection(std::vector<std::string> lineSplit, int countLine)
{
	*(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->end() - 1);
	if (lineSplit.size() <= 2) {
		std::cerr << "Invalid redirection syntax: must be return [httpCode] [file.html associed], at line " << countLine << std::endl;
		return false;
	}
	if (*(lineSplit.begin() + 1) != "301" && *(lineSplit.begin() + 1) != "302") {
		std::cerr << "Error redirection parsing: http code " << *(lineSplit.begin() + 1) << " is invalid it must be 301 or 302 at line " << countLine << std::endl;
		return false; 
	}
	lineSplit.erase(lineSplit.begin());
	setRedirection(lineSplit);
	return true;
}

bool	Location::handleCgi(std::vector<std::string> lineSplit, int countLine)
{
	(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->size() - 1);

	if ((lineSplit.end() - 1)->empty())
		lineSplit.erase(lineSplit.end() - 1);
	if (lineSplit.size() != 3) {
		std::cerr << "Error parsing Location: Cgi last arg can't be empty at line " << countLine << std::endl;
		return false;
	}
	std::cout << "je find sur " << *(lineSplit.begin() + 1) << std::endl;
	if ((lineSplit.begin() + 1)->find(".") != 0) {
		std::cerr << "Error Location Parsing: Cgi invalid mime format [" << *(lineSplit.begin() + 1) << "] at line " << countLine << std::endl;
		return false;
	}
	if (access((lineSplit.end() - 1)->c_str(), F_OK | R_OK | X_OK) == -1) {
		std::cerr << "Error Location Parsing: Cgi invalid Cgi path: [" << *(lineSplit.end() - 1) << "] " << strerror(errno) << " at line " << countLine << std::endl;
		return false;
	}
	std::cout << "cgi mime = " << *(lineSplit.begin() + 1) << " cgi path = " << *(lineSplit.begin() + 2) << std::endl;
	setCgi(*(lineSplit.begin() + 1), *(lineSplit.begin() + 2));
	return true;
}

/* -------------------------------------------------------------------- */

bool	Location::LocationParsing(std::ifstream & file, int *countLine) {

	std::string LocationKeyWord[] = {"root", "autoindex", "index", "allowedMethods",\
							 "return", "cgi"};
	bool	(Location::*FuncPtr[]) (std::vector<std::string>, int) = {&Location::handleRoot, &Location::handleAutoIndex, \
				&Location::handleIndex, &Location::handleAllowedMethods, &Location::handleRedirection, &Location::handleCgi};
	std::string 				line;
	std::vector<std::string>	lineSplit;

	while (getline(file, line)) {

		if (line.empty() || isOnlyWithSpace(line) || isCommentary(line)) {
			(*countLine)++;
			continue ;
		}
		// split la ligne
		lineSplit = split(line);
		// si la ligne est } break et sortir
		if (*(lineSplit.begin()) == "}")
			return true;

		// verifie si un mot il y a un mot sans valeur -> erreur de parsing
		if (lineSplit.size() == 1) {
			std::cerr << "Invalid location value: At line " << *countLine << " " << *(lineSplit.begin()) << " need a value" << std::endl;
			return false;
		}

		// continuer le parsing de locationName
		// check si la ligne a le bon mot cle
		for (size_t i = 0; i < sizeof(LocationKeyWord) / sizeof(std::string) ; i++) {
			// si correspondance il y a, break il y aura
			if (*(lineSplit.begin()) == LocationKeyWord[i])
			{
				if (!(this->*FuncPtr[i])(lineSplit, *countLine))
					return false;
				else
					break;
			}
			// si il n'y a aucune correspondance alors mess d'err et return
			if (i + 1 == sizeof(LocationKeyWord) / sizeof(std::string)) {
				std::cerr << "Invalid syntax: Location Invalid token [" << *(lineSplit.begin()) << ']' << " at line " << *countLine  << std::endl;
				return false;
			}
		}
		(*countLine)++;
	}
	return true;
}

std::ostream & operator<<(std::ostream & o, Location *location) {
		if (!location) {
			o << "Location est null (elle n'existe pas)" << std::endl;
			return o;
		}
		o << GREEN << "LOCATION PRINTING" << RESET << std::endl;
		o << "Allowed Method:" << std::endl; printVector(location->getAllowedMethodVector(), o);
		o << "Index:" << std::endl; printVector(location->getIndex(), o);
		o << "Redirection HTTP CODE = " << location->getRedirection("CODE") << " Redirection PATH = " << location->getRedirection("") << std::endl;
		o << "Root = " << location->getRoot() << std::endl;
		o << "Auto Index = " << location->getAutoIndex() << std::endl;
		o << "location index = " << location->getLocationID() << std::endl;
		o << "Cgi info: " << std::endl;
		if (location->getPairCgi().size() > 0) {
			for (std::vector<std::pair<std::string, std::string> >::iterator it = location->getPairCgi().begin(); it != location->getPairCgi().end(); it++) {
				o << "mime = " << it->first << " cgi path = " << it->second << std::endl;
			}
		}
		else
			o << "Cgi vector is empty" << std::endl;
	return o;
}
