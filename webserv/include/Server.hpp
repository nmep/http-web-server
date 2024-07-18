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
		static	uint16_t													_port;
		static	int															_socket;
		static	int 														_server_count;
		static	std::vector<std::string>									_serverName;
		static	std::map<std::string, std::string>							_error_page;
		static	uint16_t													_client_max_body_size;
		static	std::string													_hostName;
		static	struct sockaddr_in											_addr;
		static	bool														_autoIndex;
		std::map<const std::string, std::map<std::string, std::string>> 	_location;

	public:
		Server();
		~Server();
		// faire de la copy
		// surcharge d'op =

		uint16_t				GetPort() const;
		int						GetSocket() const;
		int						GetServerCount() const;
		std::string				GetServerName(int index) const;
		std::string				GetHostName() const;
		struct sockaddr_in		GetAddr() const;
		bool					GetAutoIndex() const;
		std::string				GetErrorPage(std::string httpCode); // to do
		uint16_t				GetClientMaxBodySize(void);

		static void				SetPort(uint16_t & val);
		static void				SetSocket(int & val);
		static void				SetServerCount(int & val);
		static void				SetServerName(std::string & val);
		static void				SetClientMaxBodySize(uint16_t & val);
		static bool				SetErrorPage(std::vector<std::string> lineSplit, int countLine);

		static void				SetHostName(std::string & val);
		static void				SetAddr(struct sockaddr_in & addr);
		static void				SetAutoIndex(int val);

		bool	ft_parse_config_file(const std::string & confFile);
};
// surcharge d'op <<

# endif