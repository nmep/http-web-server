#include "tools.hpp"

bool	strIsNum(std::string str) {
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] < '0' || str[i] > '9') {
			return false;
		}
	}
	return true;
}

bool	isOnlyWithSpace(std::string const & line)
{
	for (size_t i = 0; i < line.length(); i++) {
		if ((line[i] < 9 || line[i] > 13) && line[i] != 32)
			return false;
	}
	return true;
}

bool	isCommentary(std::string const & line) {
	// saute les white space
	size_t i = 0;
	while (i < line.size() && ((line[i] >= 9 && line[i] <= 13) || line[i] == 32 || line[i] == 127)) {
		i++;
	}
	return line[i] == '#' ? true : false;
}

std::vector<std::string>	split(std::string & line) {
	std::vector<std::string> v;
	size_t start = 0;
	size_t end = 0;

	while (end != std::string::npos)
	{
		if (line.find(9, start) < line.find(32, start))
			end = line.find(9, start);
		else
			end = line.find(32, start);
		if (end > start)
			v.push_back(line.substr(start, end - start));
		start = end + 1;
	}
	return v;
}

uint16_t	ft_atoi_port(uint16_t *ptr, std::string str) {
	uint16_t	i = 0;

	while ((str[i] > 9 && str[i] < 13) || str[i] == 32) {
		i++;
	}

	while (str[i] >= '0' && str[i] <= '9') {
		if (*ptr > PORT_MAX / 10 || (*ptr == PORT_MAX / 10 && (str[i] - 48) > PORT_MAX % 10)) {
			std::cerr << "Port set can't be more than " << PORT_MAX << std::endl;
			return false;
		}
		*ptr = *ptr * 10 + (str[i] - 48);
		i++;
	}
	if (*ptr <= 1024) {
		std::cerr << "Port set can't be less than " << 1024 << " super user permission is needed" << std::endl;
		return false;
	}
	return true;
}

bool	CheckErrorClientValue(std::string & str) {
	if ((str >= "400" && str <= "419") || (str >= "421" && str <= "429") || str == "431"
		|| (str >= "449" && str <= "451") || str == "456") {
			return true;
	}
	return false;
}

bool	checkHtmlAccess(std::string filePath) {
	std::string checkHTMLExtension = filePath;
	checkHTMLExtension = checkHTMLExtension.erase(0, checkHTMLExtension.find('.') + 1);
	if (checkHTMLExtension.empty()) {
		std::cerr << "Invalid extension file: [" << filePath << "] dosn't have a .html" << std::endl;
		return false;
	}
	if (checkHTMLExtension != "html") {
		std::cerr << "Invalid extension file: [" << filePath << "] dosn't have a .html" << std::endl;
		return false;
	}
	if (access(filePath.c_str(), F_OK | R_OK) < 0) {
		std::cerr << "Invalid syntax: " << "Access on html file [" << filePath << "]: " << strerror(errno) << std::endl;
		return false;
	}
	return true;
}

bool	checkAccessFile(std::string const & fileName, int mode)
{
	return access(fileName.c_str(), mode) == 0 ? true : false;
}

bool	StrIsContext(std::string const & str) {
	const std::string context[] = {"server", "location", "}"};

	for (size_t i = 0; i < sizeof(context) / sizeof(std::string) ; i++) {
		if (str == context[i])
			return true;
	}
	return false;
}

bool	StrSyntaxeCheck(std::string const & str) {
	return str[str.size() - 1] == ';' ? true : false;
}

bool	ft_atoi_client_max_body_size(std::string const & str, unsigned long long *ptr) {
	int i = 0;
	
	while (str[i] >= '0' && str[i] <= '9') {
		if (*ptr > ULLONG_MAX / 10 || (*ptr == ULLONG_MAX / 10 && ((unsigned long long)(str[i] - 48) > ULLONG_MAX % 10)))
			return false;
		*ptr = *ptr * 10 + (str[i] - 48);
		i++;
	}
	return true;
}

unsigned long long	convert_bytes_into_type(unsigned long long val, char type) {
	if (type == 'M')
		return val * 1048576;
	else if (type == 'K')
		return val * 1024;
	else if (type == 'B')
		return val;
	return 0;
}

bool	isDir(std::string dirName)
{
	DIR	*dir = opendir(dirName.c_str());
	if (dir == NULL) {
		std::cerr << "Error parsing isDir: " << dirName << " is not a directory" << std::endl;
		return false;
	}
	closedir(dir);
	return true;
}