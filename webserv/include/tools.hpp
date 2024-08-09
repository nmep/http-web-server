#ifndef TOOLS_HPP
	#define TOOLS_HPP

	#include "library_needed.hpp"

	#define PORT_MAX 65535

	bool						strIsNum(std::string str);
	bool						CheckErrorClientValue(std::string & str);
	bool						checkHtmlAccess(std::string filePath);
	bool 						isOnlyWithSpace(std::string const & line);
	std::vector<std::string>	split(std::string & line);
	uint16_t					ft_atoi_port(uint16_t *ptr, std::string str);
	bool						CheckErrorClientValue(std::string & str);
	bool						checkHtmlAccess(std::string filePath);


#endif