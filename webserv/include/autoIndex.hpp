#ifndef AUTOINDEX_HPP
	#define AUTOINDEX_HPP

#include <dirent.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cstring> // strlen
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <cerrno>
#include <sstream>
#include <string>

	class	AutoIndex
	{
		private:
			std::string _dirName;
			DIR *_dirPath;
			DIR *_file;
			struct stat _fileInfo;
			struct dirent *_readDir;
		public:
			AutoIndex();
			AutoIndex(std::string dirName);
			~AutoIndex();

			std::string convertFileSizeBytesIntoStr(off_t fileSize);
			std::string createHttpPage();
	};
#endif