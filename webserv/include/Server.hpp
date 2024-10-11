# ifndef SERVER_HPP
# define SERVER_HPP

	#include "library_needed.hpp"
	#include "print_tools.hpp"
	#include "tools.hpp"
	#include "Location.hpp"
	#include "autoIndex.hpp"

class Server
{
	private:
		bool									_default_server;
		uint16_t								_port;
		std::string								_serverName;
		std::string								_hostName;
		std::map<std::string, std::string>		_error_page;
		uint16_t								_client_max_body_size;
		std::map<std::string, Location*>		_location;
		bool									_autoIndex;


	public:
		Server();
		~Server();
		Server(Server const & copy);
		Server & operator=(Server const & rhs);

		// GETTEUR
		bool								GetDefaultServer() const;
		uint16_t							GetPort() const;
		std::string							GetServerName() const;
		std::string							GetHostName() const;
		std::string							GetErrorPage(std::string const & httpCode);
		uint16_t							GetClientMaxBodySize(void) const;
		std::map<std::string, std::string>	getErrorPageMap() const;
		bool								getAutoIndex() const;

		Location*							getLocation(std::string const & locationName);// ca cree une loc en plus quand le location name n'exite pas encore
		bool								isLocationExisting(std::string const & locationName) const;
		std::map<std::string, Location*>		getLocationMap() const;

		// SETTEUR
		void			SetDefaultServer();
		void			SetPort(uint16_t & val);
		void			SetServerName(std::string const & val);
		void			SetHostName(std::string const & val);
		void			SetClientMaxBodySize(uint16_t & val);
		bool			SetErrorPage(std::vector<std::string> lineSplit, int countLine);
		void			setAutoIndex(bool value);

		// FONCTIONS DE GESTION DES MOTS CLES
		bool	handleListenParsing(std::vector<std::string> lineSplit, int countLine);
		bool	handleServerNameParsing(std::vector<std::string> lineSplit, int countLine);
		bool	handleErrorPageParsing(std::vector<std::string> lineSplit, int countLine);
		bool	handleHostName(std::vector<std::string> lineSplit, int countLine);
		bool	handleClientMaxBodySizeParsing(std::vector<std::string> lineSplit, int countLine);
		bool	handleAutoIndex(std::vector<std::string> lineSplit, int countLine);
		bool 	AssignToken(std::vector<std::string> lineSplit, int countLine);

		// FONCTION PRINCIPALE DE PARSING DE SERVEUR
		bool	parseConfFile(std::ifstream & confFileFD, int *countLine);

};

std::ostream & operator<<(std::ostream& o, Server const & server);

# endif