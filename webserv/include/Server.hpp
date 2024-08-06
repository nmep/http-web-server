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
		bool									_autoIndex;

	public:
		Server();
		~Server();
		// faire de la copy
		// surcharge d'op =

		bool			GetDefaultServer() const;
		uint16_t		GetPort() const;
		std::string		GetServerName() const;
		std::string		GetHostName() const;
		bool			GetAutoIndex() const;
		std::string		GetErrorPage(std::string const & httpCode);
		uint16_t		GetClientMaxBodySize(void);

		void			SetDefaultServer();
		void			SetPort(uint16_t & val);
		void			SetServerName(std::string const & val);
		void			SetHostName(std::string const & val);
		void			SetAutoIndex(int val);
		void			SetClientMaxBodySize(uint16_t & val);
		bool			SetErrorPage(std::vector<std::string> lineSplit, int countLine);

		bool	handleListenParsing(std::vector<std::string> lineSplit, int countLine);
		bool	handleServerNameParsing(std::vector<std::string> lineSplit, int countLine);
		bool	handleErrorPageParsing(std::vector<std::string> lineSplit, int countLine);
		bool	handleAutoIndex(std::vector<std::string> lineSplit, int countLine);
		bool	handleClientMaxBodySizeParsing(std::vector<std::string> lineSplit, int countLine);


		bool 	AssignToken(std::vector<std::string> lineSplit, int countLine);
		bool	ReadFile(const std::string & confFileFD);


};
// surcharge d'op <<

# endif