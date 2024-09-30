#ifndef	LOCATION_HPP
#define	LOCATION_HPP

	#include "library_needed.hpp"
	#include "print_tools.hpp"
	#include "tools.hpp"

	class Location
	{
		private:
			std::vector<std::string>	_allowedMethod;
			std::string					_redirection[2]; // return html if it ping the location name
			std::string					_root;
			std::string					_uploadStore;
			std::string					_index;
			bool						_autoIndex;
			bool						_isUploadFileAccepted;
			static int					_locationIndex;
			int							_locationID;

		public:
			Location();
			Location(Location const & copy);
			Location & operator=(Location const & rhs);
			~Location();

			// GETTEUR
			std::vector<std::string>	getAllowedMethodVector() const;
			std::string					getRedirection(std::string const & returnValue) const;
			std::string					getRoot() const;
			std::string					getUploadStore() const;
			bool						getAllowedMethod(std::string const & method) const;
			bool						getAutoInex() const;
			bool						getIsUploadFileAccepted() const;
			int							getLocationID() const {
				return _locationID;
			}

			// SETTEUR
			void	setAllowedMethod(std::vector<std::string> const & allowedMethod);
			void	setRedirection(std::vector<std::string> const & redirection);
			void	setRoot(std::string const & root);
			void	setAutoIndex(bool value);
			void	setIsUploadFileAccepted(bool value);
			void	setUploadStore(std::string directoryUpload);
			void	setIndex(std::string const & indexFileName);

			// FONCTION DE GESTION DES MOTS CLES
			bool	handleRoot(std::vector<std::string> lineSplit, int countLine);
			bool	handleAutoIndex(std::vector<std::string> lineSplit, int countLine);
			bool	handleIndex(std::vector<std::string> lineSplit, int countLine);
			bool	handleAllowedMethods(std::vector<std::string> lineSplit, int countLine);
			bool	handleRedirection(std::vector<std::string> lineSplit, int countLine);
			bool	handleUploadStore(std::vector<std::string> lineSplit, int countLine);

			// FONCTION PRINCIPALE DE PARSING POUR UNE LOCATION
			bool	LocationParsing(std::ifstream & file, int *countLine);

			// clear memory
			// void	clearAllowedMethods();

	};

std::ostream & operator<<(std::ostream& o, Location* location);

#endif