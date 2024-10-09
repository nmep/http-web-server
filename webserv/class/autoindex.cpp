#include "autoIndex.hpp"

AutoIndex::AutoIndex() : _dirName("./"), _dirPath(NULL), _file(NULL)
{
	std::cout << "Auto index default constructor called" << std::endl;
}

AutoIndex::AutoIndex(std::string dirName) : _dirName(dirName), _dirPath(NULL), _file(NULL)
{
	std::cout << "auto index constructeur dir name called" << std::endl;
}

AutoIndex::~AutoIndex()
{
	std::cout << "Auto index destrusctor called" << std::endl;
	if (_dirPath)
		closedir(_dirPath);
	if (_file)
		closedir(_file);
}

/* ------------------------------------------------------------------------------------------------------ */

std::string	AutoIndex::convertFileSizeBytesIntoStr(off_t fileSize) {
	std::ostringstream strs;
	double value;
	
	if (fileSize >= 1099511627776) {
		value = static_cast<double>(fileSize) / 1099511627776;
		strs << std::fixed << std::setprecision(2) << value;
		return strs.str() + " Tb";
	}
	else if (fileSize >= 1073741824) {
		value = static_cast<double>(fileSize) / 1073741824;
		strs << std::fixed << std::setprecision(2) << value;
		return strs.str() + " Gb";
	}
	else if (fileSize >= 1048576) {
		value = static_cast<double>(fileSize) / 1048576;
		strs << std::fixed << std::setprecision(2) << value;
		return  strs.str() + " Mb";
	}
	else if (fileSize >= 1024) {
		value = static_cast<double>(fileSize) / 1024;
		strs << std::fixed << std::setprecision(2) << value;
		return strs.str() + " Kb";
	}
	else {
		value = static_cast<double>(fileSize);
		strs << std::fixed << std::setprecision(2) << value;
		return strs.str() + " bytes";
	}
}

std::string	AutoIndex::createHttpPage()
{
	// page http a remplir avec le contenue du dossier
	std::string	page = 
"<!doctype html>\n\
<html lang=\"fr\">\n\
	<head>\n\
	<meta charset=\"utf-8\" />\n\
		<meta name=\"viewport\" content=\"width=device-width\" />\n\
		<title>" + this->_dirName + "</title>\n\
	</head>\n\
	<body>\n\
		<h1>Auto Index of " + this->_dirName + "</h1>\n\
		<hr width=\"100%\" size=\"5\" color=\"black\">\n";

	// open le dossier
	if ((this->_dirPath = opendir(this->_dirName.c_str())) == NULL) {
		std::cerr << "Open dir failed: " << strerror(errno) << std::endl;
		return std::string(0);
	}
	
	// open le dossier
	if ((this->_file = opendir(this->_dirName.c_str())) == NULL) {
		std::cerr << "Open dir failed: " << strerror(errno) << std::endl;
		return std::string(0);
	}

	// lire dans le dossier pour afficher d'abord les dossiers et les autres types de valeurs
	while ((this->_readDir = readdir(this->_dirPath)) != NULL) {
		if (this->_readDir->d_type == DT_REG)
			continue;
		page += "<p>";
		if (this->_readDir->d_type == DT_BLK)
			page += "Block device: ";
		else if (this->_readDir->d_type == DT_CHR)
			page += "Character device: ";
		else if (this->_readDir->d_type == DT_DIR)
			page += "Directory: ";
		else if (this->_readDir->d_type == DT_FIFO)
			page += "Named pipe (FIFO): ";
		else if (this->_readDir->d_type == DT_SOCK)
			page += "Unix domain socket: ";
		else if (this->_readDir->d_type == DT_UNKNOWN)
			page += "The file type could not be determined: ";
		// ici prend state du readdir
		page += std::string(this->_readDir->d_name);

		std::string path = std::string(this->_dirName) + '/' + this->_readDir->d_name;
		if (stat(path.c_str(), &this->_fileInfo) == -1) {
			std::cerr << "Error with stat on: [" << this->_readDir->d_name << "]: " << strerror(errno) << std::endl;
			page += "</p>\n";
			continue;
		}
		// ajouter les stats
		page += " Size : " + convertFileSizeBytesIntoStr(this->_fileInfo.st_size);
		page += " Created: " + std::string(std::ctime(&this->_fileInfo.st_mtime)).substr(0, 24);
		page += "</p>\n\n";

	}
	page += "\n</body>\n\
</html>\n";
	return page;
}

// int	main()
// {
// 	AutoIndex autoindex(".");

// 	std::cout << autoindex.createHttpPage() << std::endl;
// }