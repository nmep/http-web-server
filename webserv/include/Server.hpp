# ifndef SERVER_HPP
# define SERVER_HPP

	#include "library_needed.hpp"
	#include "print_tools.hpp"
	#include "tools.hpp"
	#include "Location.hpp"

class Server
{
	private:
		bool									_default_server;
		uint16_t								_port;
		std::string								_serverName;
		std::string								_hostName;
		std::map<std::string, std::string>		_error_page;
		uint16_t								_client_max_body_size;
		std::map<std::string, Location>			_location;

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
		bool	parseConfFile(std::ifstream & confFileFD, int *countLine);
};

std::ostream & operator<<(std::ostream& o, Server const & server);

# endif