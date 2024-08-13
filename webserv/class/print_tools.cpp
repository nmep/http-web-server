#include "print_tools.hpp"

void	printVector(std::vector<std::string> const & v, std::ostream & o) {
	for (std::vector<std::string>::const_iterator it = v.begin(); it < v.end(); it++) {
		o << "v[] = " << *(it) << std::endl;
	}
}

void	printMap(std::map<std::string, std::string> map, std::ostream & o) {
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++) {
		o << "first = " << it->first << std::endl;
		o << "second = " << it->second << std::endl;
	}
}