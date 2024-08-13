#ifndef	LOCATION_HPP
#define	LOCATION_HPP

	#include "library_needed.hpp"
	#include "print_tools.hpp"
	#include "tools.hpp"

	class Location
	{
		private:
			std::vector<std::string>	_allowedMethod;
			std::string*				_redirection; // return html if it ping the location name
			std::string					_root;
			bool						_autoIndex;
			std::string					_index;
			bool						_isUploadFileAccepted;
			std::string					_uploadStore;

		public:
			Location();
			Location(Location const & copy);
			Location & operator=(Location const & rhs);
			~Location();

			bool						getAllowedMethod(std::string const & method) const;
			std::string					getRedirection(std::string const & returnValue) const;
			std::string					getRoot() const;
			bool						getAutoInex() const;
			bool						getIsUploadFileAccepted() const;
			std::string					getUploadStore() const;
			std::vector<std::string>	getAllowedMethodVector() const;

			void	setAllowedMethod(std::vector<std::string> const & allowedMethod);
			void	setRedirection(std::vector<std::string> redirection);
			void	setRoot(std::string const & root);
			void	setAutoIndex(bool value);
			void	setIsUploadFileAccepted(bool value);
			void	setUploadStore(std::string directoryUpload);
			void	setIndex(std::string const & indexFileName);

			// fonction de handle
			bool	handleRoot(std::vector<std::string> lineSplit, int countLine);
			bool	handleAutoIndex(std::vector<std::string> lineSplit, int countLine);
			bool	handleIndex(std::vector<std::string> lineSplit, int countLine);
			bool	handleAllowedMethods(std::vector<std::string> lineSplit, int countLine);
			bool	handleRedirection(std::vector<std::string> lineSplit, int countLine);
			bool	handleUploadStore(std::vector<std::string> lineSplit, int countLine);
			// fonction de lancement de location
			bool	LocationParsing(std::ifstream & file, int *countLine);
	};

std::ostream & operator<<(std::ostream& o, Location location);

#endif