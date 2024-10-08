#include <dirent.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cstring> // strlen
#include <sys/stat.h>
#include <ctime>
#include <iomanip>

#define RESET   "\033[0m"
#define BLACK   "\033[0;30m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"



// devrait peut etre renvoyer une string
void	convertBytesPrinting(off_t fileSize) {
	if (fileSize >= 1099511627776) {
		std::cout << static_cast<double>(fileSize) / 1099511627776 << " Tb";
		return ;
	}
	else if (fileSize >= 1073741824) {
		std::cout << static_cast<double>(fileSize) / 1073741824 << " Gb";
		return ;
	}
	else if (fileSize >= 1048576) {
		std::cout << static_cast<double>(fileSize) / 1048576 << " Mb";
		return ;
	}
	else if (fileSize >= 1024) {
		std::cout << static_cast<double>(fileSize) / 1024 << " Kb";
		return ;
	}
	else
		std::cout << fileSize << " b";
}

int	printSpace(int lenBefore, int spaceSize) {
	// std::cout << "\nlen before = " << lenBefore << "space size = " << spaceSize << std::endl;
	int	len = lenBefore;
	for (/**/; len < spaceSize; len++) {
		;
	}
	return len;
}

int	main(int ac, char **av)
{
	DIR *dir;
	DIR *file;
	struct stat fileInfo;
	struct dirent *readDir;

	std::cout << std::left << std::setw(30) << "Type" << std::setw(20) << "Size" << std::setw(30) << "Created" << std::endl;
	std::cout << std::string(80, '-') << std::endl;
	dir = opendir(av[1]);
	if (dir == NULL) {
		std::cout << "open dir failed: " << strerror(errno) << std::endl;
		return 2;
	}

	file = opendir(av[1]);
	if (file == NULL) {
		std::cout << "open dir failed: " << strerror(errno) << std::endl;
		return 2;
	}

	std::cout << "1" << std::endl;
	while ((readDir = readdir(dir)) != NULL) {
		if (readDir->d_type == DT_DIR) {
			std::cout << RED << "Directory: " << RESET;
			std::cout << readDir->d_name << std::endl;
			stat(readDir->d_name, &fileInfo);
			std::cout << "type: " << fileInfo.st_mode << std::endl;
			std::cout << "Created: " << std::ctime(&fileInfo.st_mtime) << std::endl;
		}
	}

	std::cout << "2" << std::endl;
	while ((readDir = readdir(file)) != NULL) {
		if (readDir->d_type == DT_DIR)
			continue;
		stat(readDir->d_name, &fileInfo);

		if(readDir->d_type == DT_REG)
			std::cout << GREEN << "File: " << RESET;
		else if (readDir->d_type == DT_BLK)
			std::cout << "blk: ";
		else if (readDir->d_type == DT_CHR)
			std::cout << "chr: ";
		else if (readDir->d_type == DT_FIFO)
			std::cout << "FIFO: ";
		else if (readDir->d_type == DT_LNK) 
			std::cout << "LNK: ";
		else if (readDir->d_type == DT_REG)
			std::cout << "REG: ";
		else if (readDir->d_type == DT_SOCK)
			std::cout << "SOCK: ";
		else if (readDir->d_type == DT_UNKNOWN)
			std::cout << "UNKNOW: ";
		std::cout << std::setw(30) << readDir->d_name;
		

		// std::cout << printSpace(std::strlen(readDir->d_name), 30) << std::endl;
		// std::cout << std::setw(printSpace(std::strlen(readDir->d_name), 30));

		std::cout << std::left << std::setw(20);
		convertBytesPrinting(fileInfo.st_size);
		// std::cout << std::setw(printSpace(std::strlen(readDir->d_name), 30));
		std::cout << std::left << std::setw(20) << "Created: " << std::string(std::ctime(&fileInfo.st_mtime)).substr(0, 24) << std::endl;
	}

	closedir(dir);
	closedir(file);
}