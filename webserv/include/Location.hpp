#ifndef LOCATION_HPP
#define	LOCATION_HPP

	#include "library_needed.hpp"
	#include "print_tools.hpp"

	class Location
	{
		private:
			std::vector<std::string>	_allowedMethod;
			std::string*				_redirection;
			std::string					_root;
			bool						_autoIndex;
			bool						_isUploadFileAccepted;
			std::string					_directoryUpload;

		public:
			Location();
			Location(Location const & copy);
			Location & operator=(Location const & rhs);
			~Location();

			// qwrqwerwerwqrr
			bool						getAllowedMethod(std::string const & method) const;
			std::string				getRedirection(std::string const & returnValue) const;
			std::string					getRoot() const;
			bool						getAutoInex() const;
			bool						getIsUploadFileAccepted() const;
			std::string					getDirectoryUpload() const;

			std::vector<std::string>	getAllowedMethodVector() const;

			// setteur
			void	setAllowedMethod(std::vector<std::string> const & allowedMethod);
			void	setRedirection(std::string* redirection);
			void	setRoot(std::string const & root);
			void	setAutoIndex(bool value);
			void	setIsUploadFileAccepted(bool value);
			void	setDirectoryUpload(std::string directoryUpload);
	};

std::ostream & operator<<(std::ostream& o, Location const & location);

#endif