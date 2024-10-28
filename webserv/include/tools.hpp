#ifndef TOOLS_HPP
	#define TOOLS_HPP

	#include "library_needed.hpp"

	#define PORT_MAX 65535

	bool						strIsNum(std::string str);
	bool						CheckErrorClientValue(std::string & str);
	bool						checkHtmlAccess(std::string filePath);
	bool 						isOnlyWithSpace(std::string const & line);
	bool						isCommentary(std::string const & line);
	std::vector<std::string>	split(std::string & line);
	uint16_t					ft_atoi_port(uint16_t *ptr, std::string str);
	bool						ft_atoi_client_max_body_size(std::string const & str, unsigned long long *ptr);
	unsigned long long			convert_bytes_into_type(unsigned long long val, char type);
	bool						CheckErrorClientValue(std::string & str);
	bool						checkHtmlAccess(std::string filePath);
	bool						StrIsContext(std::string const & str);
	bool						StrSyntaxeCheck(std::string const & str);
	bool						checkAccessFile(std::string const & fileName, int mode);
	bool						isDir(std::string dirName);

#endif