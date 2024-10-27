#include "configuration.hpp"

Configuration::Configuration() : _nbServer(0), _nbPort(0)
{
	std::cout << RED << "Configuration Constructeur called" << RESET << std::endl;
	this->_syntaxData.OCB = 0;
	this->_syntaxData.CCB = 0;
	this->_syntaxData.CountLine = 1;
	this->_servTab = NULL;
}

Configuration::~Configuration()
{
	std::cout << RED << "Configuration destructor called" << RESET  << std::endl;
	delete[] _servTab;
}

Configuration::Configuration(Configuration const & copy)
{
	*this = copy;
}

Configuration & Configuration::operator=(Configuration const & rhs)
{
	_nbServer = rhs._nbServer;
	_syntaxData = rhs._syntaxData;

	if (_servTab)
		delete[] _servTab;
	_servTab = new Server[rhs._nbServer];
	for (int i = 0; i < rhs._nbServer; i++) {
		_servTab[i] = rhs._servTab[i];
	}

	_confFileName = rhs._confFileName;
	return *this;
}

/* ------------------------------------------------------------------------------------------------- */

int	Configuration::getNbServer() const
{
	return _nbServer;
}

int	Configuration::getNbPort() const
{
	return _nbPort;
}

t_syntaxParse	Configuration::getSyntaxData() const
{
	return _syntaxData;
}

Server&	Configuration::getServer(int index) const
{
	return _servTab[index];
}

std::string Configuration::getConfFileName() const
{
	return _confFileName;
}

/* ------------------------------------------------------------------------------------------------- */

void	Configuration::setNBServer(int nbServer)
{
	_nbServer = nbServer;
}

bool	Configuration::syntaxParse(std::vector<std::string> lineSplit)
{
	const std::string cTokens[] = {"server", "listen", "server_name",
	 "error_page", "client_max_body_size", "location", "allowedMethods", "autoindex", "}", "root", "return", "index", "upload_store", "//", "#"};

	std::vector<std::string>::const_iterator ite = lineSplit.end();

	// verifier si le mot est valide
	for (size_t i = 0; i < sizeof(cTokens) / sizeof(cTokens[0]) ; i++) {
		if (*(lineSplit.begin()) == cTokens[i])
			break ;
		if (i + 1 == sizeof(cTokens) / sizeof(cTokens[0])) {
			std::cerr << "Invalid syntax: Invalid token [" << *(lineSplit.begin()) << "] at line " << this->_syntaxData.CountLine << std::endl;
			return false;
		}
	}

	if (StrIsContext(*(lineSplit.begin())))
	{
		if (*(ite - 1) != "{" && *(ite - 1) != "}")
		{
			std::cerr << "Invalid syntax: " << *(ite - 1) << " at line " << this->_syntaxData.CountLine \
				<< " curly brace is missing" << std::endl;
				return false;
		}
	}
	else
	{
		if (!StrSyntaxeCheck(*(ite - 1)))
		{
			std::cerr << "Invalid syntax: " << *(ite - 1) << " at line " << this->_syntaxData.CountLine \
				<< " ';' is missing at the end of line" << std::endl;
			return false;
		}
	}
	for (std::vector<std::string>::const_iterator it = lineSplit.begin(); it < lineSplit.end(); it++) {
		if (*(it) == "{}")
		{
			std::cerr << "Invalid syntax: " << *(it) << " at line " << this->_syntaxData.CountLine << std::endl;
			return false;
		}
		else if (*(it) == "{")
			this->_syntaxData.OCB++;
		else if (*(it) == "}")
			this->_syntaxData.CCB++;
	}
	return true;
}

/* ------------------------------------------------------------------------------------------------- */

// lancer le read file, ca lit une premiere fois la syntax puis ca le refait et ca prend en memoire
bool	Configuration::readFileSyntax()
{
	std::string line;
	std::ifstream confFileFD(_confFileName.c_str());
	std::vector<std::string> lineSplit;

	int in_scope = 0;
	while (getline(confFileFD, line))
	{
		if (line.empty() || isOnlyWithSpace(line) || isCommentary(line)) {
			this->_syntaxData.CountLine++;
			continue ;
		}
		lineSplit = split(line);
		if (*(lineSplit.begin()) == "server") {
			in_scope = 1;
			this->_nbServer++;
		}
		// check si la ligne est en dehors du scope des accolade, si oui mettre faux, ca sert a n'avoir que des directive principales de serveur
		// si ocb et ccb s
		if (in_scope == 0) {
			std::cerr << "Invalid syntax: element -> \'" << line << "\' isn't in the scope at the line " << this->_syntaxData.CountLine << std::endl;   
			return false;
		}

		// check si le premier mot est correct
		if (!syntaxParse(lineSplit))
			return false;
		this->_syntaxData.CountLine++;

		// dans le scope ?
		if (this->_syntaxData.CCB == this->_syntaxData.OCB) {
			in_scope = 0;
		}
	}

	if (this->_syntaxData.OCB != this->_syntaxData.CCB) {
		if (this->_syntaxData.OCB < this->_syntaxData.CCB)
			std::cerr << "Invalid syntax: " << this->_syntaxData.CCB - this->_syntaxData.OCB << " Open curly brace is missing" << std::endl;
		else
			std::cerr << "Invalid syntax: " << this->_syntaxData.OCB - this->_syntaxData.CCB << " Close curly brace is missing" << std::endl;
		return false;
	}
	return true;
}

// fonction principale du parsing
bool	Configuration::launchServerConf(const std::string & confFileName)
{
	if (!checkAccessFile(confFileName, F_OK | R_OK)) {
		std::cerr << "Invalid configuration file [" << confFileName << "]: " << strerror(errno) << std::endl;
		return false;
	}
	_confFileName = confFileName;
	std::string line;
	std::ifstream confFileFD(confFileName.c_str());
	std::vector<std::string> lineSplit;
	int serverToConf = 0;
	int countLine = 1;

	if (!readFileSyntax())
		return false;

	if (_nbServer == 0) {
		std::cerr << "Error parsing: No server scope detected, invalid configuration file" << std::endl;
		return false;
	}
	_servTab = new Server[_nbServer];
	// lancer la lecture de mot et si je trouve le mot server je lance la config d'un server
	// 		(important: il faut que ce soit avec le meme fd pour etre sur la meme ligne)
	while (getline(confFileFD, line)) {
		if (line.empty() || isOnlyWithSpace(line) || isCommentary(line)) {
			countLine++;
			continue;
		}
		lineSplit = split(line);
		if (*(lineSplit.begin()) == "server") {
			// mettre le premier serveur rencontre comme serveur par defaut
			if (serverToConf == 0) {
				_servTab[serverToConf].SetDefaultServer(); // TO DO enlever ca
			}
			// lancer la conf du serveur
			countLine++;
			if (!_servTab[serverToConf].parseConfFile(confFileFD, &countLine)) {
				// ici set l'idx du serv
				_servTab[serverToConf].setServerIdx(serverToConf);
				return false;
			}
			// ici set l'idx du serv
			_servTab[serverToConf].setServerIdx(serverToConf);
			serverToConf++;
			countLine++;
		}
	}
	// conf finit
	std::cout << "nb serv = " << _nbServer << std::endl;
	for (int i = 0; i < _nbServer; i++) {
		_nbPort += getServer(i).GetPortVector().size();
	}
	return true;
}

std::ostream & operator<<(std::ostream & o, Configuration const & conf)
{
	o << RED << "CONFIGURATION PRINTING" << RESET << std::endl;
	o << "Nb server = " << conf.getNbServer() << std::endl;
	o << "Syntax Data -> Open curly brace = " << conf.getSyntaxData().OCB << std::endl;
	o << "Syntax Data -> Close curly brace = " << conf.getSyntaxData().CCB << std::endl;
	o << "Syntax Data -> Count line = " << conf.getSyntaxData().CountLine << std::endl;
	o << "Configuration file name = " << conf.getConfFileName() << std::endl;
	o << "Nb Port = " << conf.getNbPort() << std::endl;
	o << "Server info:" << std::endl;
	for (int i = 0; i < conf.getNbServer(); i++) {
		o << BLUE << "\tServer[" << i << "]: " << RESET << conf.getServer(i) << std::endl;
	}
	return o;
}