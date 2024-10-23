#include "print_tools.hpp"

void	printMap(std::map<std::string, std::string> map, std::ostream & o) {
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++) {
		o << "first = " << it->first << std::endl;
		o << "second = " << it->second << std::endl;
	}
}