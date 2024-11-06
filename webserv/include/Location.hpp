#ifndef	LOCATION_HPP
#define	LOCATION_HPP

	#include "library_needed.hpp"
	#include "print_tools.hpp"
	#include "tools.hpp"

	class Location
	{
		private:
			std::vector<std::string>							_allowedMethod;
			std::string											_redirection[2]; // return html if it ping the location name
			std::string											_root;
			std::vector<std::string>							_index;
			bool												_autoIndex;
			static int											_locationIndex;
			int													_locationID;
			std::vector<std::pair<std::string, std::string> >	_cgi;

		public:
			Location();
			Location(Location const & copy);
			Location & operator=(Location const & rhs);
			~Location();

			// GETTEUR
			std::vector<std::string>							getAllowedMethodVector() const;
			std::vector<std::string>							getIndex() const;
			std::string											getRedirection(std::string const & returnValue) const;
			std::string											getRoot() const;
			bool												getAllowedMethod(std::string const & method) const;
			bool												getAutoIndex() const;
			int													getLocationID() const;
			std::vector<std::pair<std::string, std::string> > &	getPairCgi();

			// SETTEUR
			void	setAllowedMethod(std::vector<std::string> const & allowedMethod);
			void	setRedirection(std::vector<std::string> const & redirection);
			void	setRoot(std::string const & root);
			void	setAutoIndex(bool value);
			void	setCgi(std::string mime, std::string cgiPath);
			void	setIndex(std::string const & indexFileName);

			bool	isCgiMimeAccepted(std::string cgiMime) const;
			bool	isCgiPathAccepted(std::string path) const;

			// FONCTION DE GESTION DES MOTS CLES
			bool	handleRoot(std::vector<std::string> lineSplit, int countLine);
			bool	handleAutoIndex(std::vector<std::string> lineSplit, int countLine);
			bool	handleIndex(std::vector<std::string> lineSplit, int countLine);
			bool	handleAllowedMethods(std::vector<std::string> lineSplit, int countLine);
			bool	handleRedirection(std::vector<std::string> lineSplit, int countLine);
			bool	handleCgi(std::vector<std::string> lineSplit, int countLine);

			// FONCTION PRINCIPALE DE PARSING POUR UNE LOCATION
			bool	LocationParsing(std::ifstream & file, int *countLine);

			// clear memory
			// void	clearAllowedMethods();
	};

std::ostream & operator<<(std::ostream& o, Location* location);

#endif