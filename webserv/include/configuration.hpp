#ifndef CONFIGURATION_HPP
	#define CONFIGURATION_HPP

	#include "library_needed.hpp"
	class Configuration
	{
		private:
			int	_nbServer;


		public:
			Configuration();
			~Configuration();
			Configuration(Configuration const & copy);
			Configuration & operator=(Configuration const & rhs);

			void	setNBServer(int nbServer);
			bool	readFile(const std::string const & confFileFD);
			bool	syntaxePArse(std::vector<std::string> const & v, int countLine);
	};

#endif