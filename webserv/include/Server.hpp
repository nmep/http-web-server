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

class Server
{
	private:
		
		uint16_t			_port;
		int					_socket;
		int 				_server_count;
		std::string			_serverName;
		std::string			_hostName;
		struct sockaddr_in	_addr;
		bool				_autoIndex;

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

		void				SetPort(uint16_t & val);
		void				SetSocket(int & val);
		void				SetServerCount(int & val);
		void				SetServerName(std::string & val);
		void				SetHostName(std::string & val);
		void				SetAddr(struct sockaddr_in & addr);
		void				SetAutoIndex(int val);

		bool				ft_parse_config_file(const std::string & confFile);
};

// surcharge d'op <<

# endif