#include "Location.hpp"

Location::Location() : _autoIndex(false), _isUploadFileAccepted(false)
{
	std::cout << GREEN << "location default constructor called" << RESET << std::endl;
}

Location::Location(Location const & copy) :_autoIndex(false), _isUploadFileAccepted(false)
{
	*this = copy;
}

Location & Location::operator=(Location const & rhs)
{
	_allowedMethod = rhs._allowedMethod;
	if (!rhs._redirection->empty()) {
		_redirection[0] = rhs._redirection[0];
	}
		_redirection[1] = rhs._redirection[1];
	_root = rhs._root;
	_autoIndex = rhs._autoIndex;
	_isUploadFileAccepted = rhs._isUploadFileAccepted;
	_uploadStore = rhs._uploadStore;
	return *this;
}

Location::~Location() {
	std::cout << GREEN << "Location destructeur called" << RESET << std::endl;
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

bool Location::getAutoInex() const {
	return _autoIndex;
}

bool Location::getIsUploadFileAccepted() const {
	if (getAllowedMethod("POST"))
		return true;
	return false;
}

std::string Location::getUploadStore() const {
	return _uploadStore;
}

std::vector<std::string> Location::getAllowedMethodVector() const {
	return _allowedMethod;
}

/* -------------------------------------------------------------------- */

void	Location::setAllowedMethod(std::vector<std::string> const & allowedMethod) {
	_allowedMethod.clear();
	// printVector(allowedMethod, std::cout);
	std::vector<std::string>::const_iterator _it = _allowedMethod.begin();
	for (std::vector<std::string>::const_iterator it = allowedMethod.begin(); it != allowedMethod.end(); it++) {
		_it = it;
		_it++;
	}
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

void	Location::setIsUploadFileAccepted(bool value) {
	_isUploadFileAccepted = value;
}

void	Location::setUploadStore(std::string directoryUpload) {
	_uploadStore = directoryUpload;
}

void	Location::setIndex(std::string const & indexFileName) {
	_index = indexFileName;
}

/* -------------------------------------------------------------------- */

bool	Location::handleAutoIndex(std::vector<std::string> lineSplit, int countLine) {
	std::cout << "handle autoindex" << std::endl;
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid syntax: at line " << countLine << " Autoindex need a value (on or off)" << std::endl;
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
		setAutoIndex(1);
	else
		setAutoIndex(0);
	return true;
}

bool	Location::handleRoot(std::vector<std::string> lineSplit, int countLine)
{
	std::cout << "handle Root" << std::endl;
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid Root syntax it must be root <root_path>, at line " << countLine << std::endl;
		return false;
	}
	(lineSplit.begin() + 1)->erase((lineSplit.begin() + 1)->end() - 1);
	if (!checkAccessFile(*(lineSplit.begin() + 1), F_OK | R_OK)) {
		std::cerr << "Invalid root path -> [" << *(lineSplit.begin() + 1) << "] " << strerror(errno) << " at line " << countLine << std::endl;
		return false;
	}
	setRoot(*(lineSplit.begin() + 1));
	return true;
}

bool	Location::handleIndex(std::vector<std::string> lineSplit, int countLine)
{
	std::cout << "handle Index" << std::endl;
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid index syntax it must be index <index_path>, at line " << countLine << std::endl;
		return false;
	}
	// erase ;
	(lineSplit.begin() + 1)->erase((lineSplit.begin() + 1)->end() - 1);
	if (!checkHtmlAccess(*(lineSplit.begin() + 1)))
		return false;
	setIndex(*(lineSplit.begin() + 1));
	return true;
}

bool	Location::handleAllowedMethods(std::vector<std::string> lineSplit, int countLine)
{
	std::cout << "handle AllowedMethods" << std::endl;
	if (lineSplit.begin()->size() == 1) {
		std::cerr << "Invalid Allowed methods syntax: no value associate, at line " << countLine << std::endl;
		return false;
	}
	// erase le point virgule
	(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->end() - 1);
	for (std::vector<std::string>::iterator it = (lineSplit.begin() + 1); it != lineSplit.end(); ++it) {
		if (*it != "GET" && *it != "POST") {
			std::cerr << "Invalid Allowed methods syntax: [" << *it << "] isn't accepted it must be GET or POST, at line " << countLine << std::endl;
			return false;
		}
		if (*it == "POST")
			setIsUploadFileAccepted(true);
	}
	// erase le premier mot
	lineSplit.erase(lineSplit.begin());
	setAllowedMethod(lineSplit);
	return true;
}

bool	Location::handleRedirection(std::vector<std::string> lineSplit, int countLine)
{
	std::cout << "handle Redirection" << std::endl;
	if (lineSplit.begin()->size() == 1) {
		std::cerr << "Invalid redirection syntax: no value associate, at line " << countLine << std::endl;
		return false;
	}
	// erase ;
	*(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->end() - 1);
	if (!checkHtmlAccess(*(lineSplit.begin() + 2))) 
		return false;

	// erase le premier 
	lineSplit.erase(lineSplit.begin());
	setRedirection(lineSplit);
	return true;
}

bool Location::handleUploadStore(std::vector<std::string> lineSplit, int countLine)
{
	if (lineSplit.size() != 2) {
		std::cerr << "Invalid upload_store syntax: no value associate, at line " << countLine << std::endl;
		return false;
	}
	*(lineSplit.end() - 1)->erase((lineSplit.end() - 1)->end() - 1);	
	// check if dir exist
	if (!checkAccessFile(*(lineSplit.begin() + 1), F_OK | R_OK | W_OK)) {
		std::cerr << "Invalid upload_store syntax: " << strerror(errno) << " at line " << countLine << std::endl;
		return false;
	}
	setUploadStore(*(lineSplit.begin() + 1));
	return true;
}

/* -------------------------------------------------------------------- */

bool	Location::LocationParsing(std::ifstream & file, int *countLine) {

	std::string LocationKeyWord[] = {"root", "auto_index", "index", "allowedMethods", "return"};
	bool	(Location::*FuncPtr[]) (std::vector<std::string>, int) = {&Location::handleRoot, &Location::handleAutoIndex, \
				&Location::handleIndex, &Location::handleAllowedMethods, &Location::handleRedirection};
	std::string 				line;
	std::vector<std::string>	lineSplit;

	while (getline(file, line)) {
		// ligne vide?
		std::cout << "line dans LOCATION = " << line << std::endl;
		if (line.empty() || isOnlyWithSpace(line)) {
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
				std::cerr << "Invalid syntax: Location Invalid token [" << *(lineSplit.begin()) << ']' << std::endl;
				return false;
			}
		}
		(*countLine)++;
	}
	return true;
}

void	Location::clearAllowedMethods() {
	this->_allowedMethod.clear();
}

std::ostream & operator<<(std::ostream & o, Location location) {
		o << "LOCATION PRINTING" << std::endl;
		o << "Allowed Method = "; printVector(location.getAllowedMethodVector(), o);
		o << "Redirection HTTP CODE = " << location.getRedirection("CODE") << " Redirection PATH = " << location.getRedirection("") << std::endl;
		o << "Root = " << location.getRoot() << std::endl;
		o << "Auto Index = " << location.getAutoInex() << std::endl;
		o << "IsUploadFileAccepted = " << location.getIsUploadFileAccepted() << std::endl;
		o << "Upload store = " << location.getUploadStore() << std::endl;
	return o;
}


