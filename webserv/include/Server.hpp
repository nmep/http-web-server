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
		int 				_server_count;
		std::string			_serverName;
		std::string			_hostName;
		struct sockaddr_in	_addr;

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

		void				SetPort(uint16_t & val);
		void				SetSocket(int & val);
		void				SetServerCount(int & val);
		void				SetServerName(std::string & val);
		void				SetHostName(std::string & val);
		void				SetAddr(struct sockaddr_in & addr);

		void				ft_parse_config_file(const std::string & confFile);
};

// surcharge d'op <<

# endif