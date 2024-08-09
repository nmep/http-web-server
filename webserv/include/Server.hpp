# ifndef SERVER_HPP
	# define SERVER_HPP

	#include <sys/socket.h> // socket + network function
	#include <netinet/in.h> // netwok

	#include <iostream> // std
	#include <sstream> // type for file input
	#include <exception> // exception
	#include <stdint.h> // uint16_t
	#include <string> // getline
	#include <fstream> // inputfile
	#include <string> // string manip

	#include <map> // map
	#include <vector> // vector

	#include <unistd.h> // access
	#include <stdio.h> // perror
	#include <string.h> // strerror

	#include <limits.h>

	#define PORT_MAX 65535

class Server
{
	private:
		bool									_default_server;
		uint16_t								_port;
		std::string								_serverName;
		std::string								_hostName;
		std::map<std::string, std::string>		_error_page;
		uint16_t								_client_max_body_size;

	public:
		Server();
		~Server();
		Server(Server const & copy);
		Server & operator=(Server const & rhs);

		bool			GetDefaultServer() const;
		uint16_t		GetPort() const;
		std::string		GetServerName() const;
		std::string		GetHostName() const;
		std::string		GetErrorPage(std::string const & httpCode);
		uint16_t		GetClientMaxBodySize(void) const;
		std::map<std::string, std::string> getErrorPageMap() const;

		void			SetDefaultServer();
		void			SetPort(uint16_t & val);
		void			SetServerName(std::string const & val);
		void			SetHostName(std::string const & val);
		void			SetClientMaxBodySize(uint16_t & val);
		bool			SetErrorPage(std::vector<std::string> lineSplit, int countLine);

		bool	handleListenParsing(std::vector<std::string> lineSplit, int countLine);
		bool	handleServerNameParsing(std::vector<std::string> lineSplit, int countLine);
		bool	handleErrorPageParsing(std::vector<std::string> lineSplit, int countLine);
		bool	handleHostName(std::vector<std::string> lineSplit, int countLine);
		bool	handleClientMaxBodySizeParsing(std::vector<std::string> lineSplit, int countLine);
		bool 	AssignToken(std::vector<std::string> lineSplit, int countLine);
		bool	parseConfFile(const std::string & confFileFD);
};

// surcharge d'op <<
// void	printVector(std::vector<std::string> v);
// void	printLocation(std::map<std::string, std::map<std::string, std::vector<std::string> > > map);
// void	printMap(std::map<std::string, std::string> map);

std::ostream & operator<<(std::ostream& o, Server const & server);

# endif