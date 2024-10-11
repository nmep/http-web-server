#include "autoIndex.hpp"

AutoIndex::AutoIndex() : _rootName("./"), _dirPath(NULL), _file(NULL)
{
	std::cout << "Auto index default constructor called" << std::endl;
}

AutoIndex::AutoIndex(std::string urlName , std::string rootName) : _urlName(urlName), _rootName(rootName), _dirPath(NULL), _file(NULL)
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


/*
si je veux que le contenu du dossier puisse etre des bouttons retournant sur 
leurs contenue je dois faire

	<a href=>
*/
std::string	AutoIndex::createHttpPage()
{
	// page http a remplir avec le contenue du dossier
	std::string	page = 
"<!doctype html>\n\
<html lang=\"fr\">\n\
	<head>\n\
	<meta charset=\"utf-8\" />\n\
		<meta name=\"viewport\" content=\"width=device-width\" />\n\
		<title>" + this->_rootName + "</title>\n\
		<style>\n\
			table { width: 100%; border-collapse: collapse; }\n\
			th, td { padding: 8px 12px; border: 1px solid #ddd; text-align: left; }\n\
			th { background-color: #f2f2f2; }\n\
		</style>\n\
	</head>\n\
	<body>\n\
		<h1>Auto Index of " + this->_rootName + "</h1>\n\
		<hr width=\"100%\" size=\"5\" color=\"black\">\n\
		<table>\n\
			<tr>\n\
				<th>Type</th>\n\
				<th>Name</th>\n\
				<th>Size</th>\n\
				<th>Last Modification</th>\n\
			</tr>\n";

	// open le dossier
	if ((this->_dirPath = opendir(this->_rootName.c_str())) == NULL) {
		std::cerr << "Open dir failed: " << strerror(errno) << std::endl;
		return std::string();
	}

	// open le dossier
	if ((this->_file = opendir(this->_rootName.c_str())) == NULL) {
		std::cerr << "Open dir failed: " << strerror(errno) << std::endl;
		return std::string();
	}

	// 1 lire dans le dossier pour afficher d'abord les dossiers et les autres types de valeurs
	while ((this->_readDir = readdir(this->_dirPath)) != NULL) {
		if (this->_readDir->d_type != DT_DIR)
			continue;

		page += "<tr>\n<td>";
		page += "📂 ";
		// ici prend state du readdir
		page += "</td>";
		page += "<td> <a href=\"" + this->_urlName + '/' +  std::string(this->_readDir->d_name) + "\">" + this->_readDir->d_name + "</a>" + "</td>";

		// ajouter le suffix du path aux file trouver dans le directory pour que stat fonction sinon il cherche dans la cwd
		std::string path = std::string(this->_rootName) + '/' + this->_readDir->d_name;
		if (stat(path.c_str(), &this->_fileInfo) == -1) {
			std::cerr << "Error with stat on: [" << this->_readDir->d_name << "]: " << strerror(errno) << std::endl;
			page += "\n</tr>\n";
			continue;
		}
		// ajouter les stats
		page += "<td>" + convertFileSizeBytesIntoStr(this->_fileInfo.st_size) + "</td>"; // size
		page += "<td>" + std::string(std::ctime(&this->_fileInfo.st_mtime)) + "</td>\n"; // last modification
		page += "</tr>\n";
	}

	// 2 lire tout les autres types de fichiers que dossier
	while ((this->_readDir = readdir(this->_file))) {
		if (this->_readDir->d_type == DT_DIR)
			continue;
		page += "<tr>\n<td>";

		if (this->_readDir->d_type == DT_REG)
			page += "📝 ";
		else if (this->_readDir->d_type == DT_BLK)
			page += "Block device: ";
		else if (this->_readDir->d_type == DT_CHR)
			page += "Character device: ";
		else if (this->_readDir->d_type == DT_FIFO)
			page += "Named pipe (FIFO): ";
		else if (this->_readDir->d_type == DT_SOCK)
			page += "Unix domain socket: ";
		else if (this->_readDir->d_type == DT_UNKNOWN)
			page += "The file type could not be determined: ";
		page += "</td>";
		page += "<td> <a href=\"" + this->_urlName + '/' +  std::string(this->_readDir->d_name) + "\">" + this->_readDir->d_name + "</a>" + "</td>";

		// ici prend state du readdir
		// ajouter le suffix du path aux file trouver dans le directory pour que stat fonction sinon il cherche dans la cwd
		std::string path = std::string(this->_rootName) + '/' + this->_readDir->d_name;
		if (stat(path.c_str(), &this->_fileInfo) == -1) {
			std::cerr << "Error with stat on: [" << this->_readDir->d_name << "]: " << strerror(errno) << std::endl;
			page += "\n</tr>\n";
			continue;
		}
		// ajouter les stats
		page += "<td>" + convertFileSizeBytesIntoStr(this->_fileInfo.st_size) + "</td>";
		page += "<td>" + std::string(std::ctime(&this->_fileInfo.st_mtime)) + "</td>\n";
		page += "</tr>\n";

	}
	page += "\n</body>\n\
	</html>\n";
	closedir(_dirPath);
	this->_dirPath = NULL;
	closedir(_file);
	this->_file = NULL;

	return page;
}