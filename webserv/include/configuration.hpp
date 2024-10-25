#ifndef CONFIGURATION_HPP
	#define CONFIGURATION_HPP

	#include "library_needed.hpp"
	#include "tools.hpp"
	#include "Server.hpp" // Server include location.hpp

	#ifndef G_SIGNAL
		#define G_SIGNAL
		extern int g_loop;
	#endif

	// cette struct recupere les quelque info dont j'ai besoin pour gerer le parsing du fichier
	typedef	struct s_syntaxParse {
		int	OCB; // open curly brace
		int	CCB; // close curly brace
		int	CountLine;
	} t_syntaxParse;

	class Configuration
	{
		private:
			int					_nbServer; // compte le nombre de server lors de la premiere lecture
			int					_nbPort;
			t_syntaxParse		_syntaxData;
			Server*				_servTab;
			std::string			_confFileName; // pas sur que ce soit utile

		public:
			Configuration();
			~Configuration();
			Configuration(Configuration const & copy);
			Configuration & operator=(Configuration const & rhs);

			int				getNbServer() const;
			int				getNbPort() const;
			t_syntaxParse	getSyntaxData() const;
			Server&			getServer(int index) const;
			std::string		getConfFileName() const;

			void	setNBServer(int nbServer);
			bool	syntaxParse(std::vector<std::string> v);
			bool	readFileSyntax(); 
			bool	launchServerConf(const std::string & confFileName);
	};

std::ostream & operator<<(std::ostream & o, Configuration const & conf);

#endif