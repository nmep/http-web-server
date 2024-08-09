#include "Location.hpp"

Location::Location() : _autoIndex(false), _isUploadFileAccepted(false)
{
	_redirection = new std::string[2];
}

Location::Location(Location const & copy)
{
	*this = copy;
}
Location & Location::operator=(Location const & rhs)
{
	_allowedMethod = rhs._allowedMethod;
	_redirection[0] = rhs._redirection[0];
	_redirection[1] = rhs._redirection[1];
	_root = rhs._root;
	_autoIndex = rhs._autoIndex;
	_isUploadFileAccepted = rhs._isUploadFileAccepted;
	_directoryUpload = rhs._directoryUpload;
	return *this;
}

Location::~Location(){
	delete _redirection;
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

std::string Location::getDirectoryUpload() const {
	return _directoryUpload;
}

std::vector<std::string> Location::getAllowedMethodVector() const {
	return _allowedMethod;
}

/* -------------------------------------------------------------------- */

void	Location::setAllowedMethod(std::vector<std::string> const & allowedMethod) {
	_allowedMethod = allowedMethod;
}

void	Location::setRedirection(std::string* redirection) {
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

void	Location::setDirectoryUpload(std::string directoryUpload) {
	_directoryUpload = directoryUpload;
}

std::ostream & operator<<(std::ostream & o, Location const & location) {
	o << "LOCATION PRINTING" << std::endl;
	o << "Allowed Method = "; printVector(location.getAllowedMethodVector(), o);
	o << "Redirection HTTP CODE = " << location.getRedirection("CODE") << " Redirection PATH = " << location.getRedirection("") << std::endl;
	o << "Root = " << location.getRoot() << std::endl;
	o << "Auto Index = " << location.getAutoInex() << std::endl;
	o << "IsUploadFileAccepted = " << location.getIsUploadFileAccepted() << std::endl;
	o << "Directory upload = " << location.getDirectoryUpload() << std::endl;
	return o;
}


