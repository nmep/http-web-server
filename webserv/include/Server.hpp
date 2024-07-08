# ifndef SERVER_HPP
	# define SERVER_HPP

	#include <stdint.h>
	#include <iostream>
	#include <map>
	#include <sys/socket.h>
	#include <netinet/in.h>

class Server
{
	private:
		
		uint16_t			_port;
		int					_socket;
		const std::string	_serverName;
		const std::string	_hostName;
		struct sockaddr_in	_addr;

		std::map<const std::string, std::string> _location;

	public:
		Server();
		~Server();
		// faire de la copy
		// surcharge d'op =

		uint16_t	GetPort() const;
		int			GetSocket() const;
		const std::string	GetServerName() const;
		const std::string	GetHostName() const;
		struct sockaddr_in	GetAddr() const;
};

// surcharge d'op <<

# endif