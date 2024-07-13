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

	#include <map> // map
	#include <vector> // vector

	#include <unistd.h> // access
	#include <stdio.h> // perror

	#include <limits.h>

	#define PORT_MAX 65535

class Server
{
	private:
		static uint16_t			_port;
		static int					_socket;
		static int 				_server_count;
		static std::string			_serverName;
		static std::string			_hostName;
		static struct sockaddr_in	_addr;
		static bool				_autoIndex;

		std::map<const std::string, std::string> _location;

	public:
		Server();
		~Server();
		// faire de la copy
		// surcharge d'op =

		uint16_t			GetPort() const;
		int					GetSocket() const;
		int					GetServerCount() const;
		const std::string	GetServerName() const;
		const std::string	GetHostName() const;
		struct sockaddr_in	GetAddr() const;
		bool				GetAutoIndex() const;

		static void				SetPort(uint16_t & val);
		static void				SetSocket(int & val);
		static void				SetServerCount(int & val);
		static void				SetServerName(std::string & val);
		static void				SetHostName(std::string & val);
		static void				SetAddr(struct sockaddr_in & addr);
		static void				SetAutoIndex(int val);

		bool	ft_parse_config_file(const std::string & confFile);
};
// surcharge d'op <<

# endif