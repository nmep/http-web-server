#include "configuration.hpp"

Configuration::Configuration() : _nbServer(0) {}

Configuration::~Configuration() {}

Configuration::Configuration(Configuration const & copy)
{
	*this = copy;
}

Configuration & Configuration::operator=(Configuration const & rhs)
{
	_nbServer = rhs._nbServer;
	return *this;
}

void	Configuration::setNBServer(int nbServer)
{
	_nbServer = nbServer;
}

bool	Configuration::readFile(const std::string const & confFileFD)
{
	// to do
}

bool	Configuration::syntaxePArse(std::vector<std::string> const & v, int countLine)
{
		std::vector<std::string>::const_iterator ite = v.end();
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

