#ifndef PRINT_TOOLS_HPP
#define PRINT_TOOLS_HPP

#include "library_needed.hpp"

template <class T>
void	printVector(std::vector<T> v, std::ostream & o) {
	//std::cout  << "debut de print vector" << std::endl;

	if (v.empty()) {
		//std::cout  << "le vector est vide je le print pas" << std::endl;
	}
	for (typename std::vector<T>::iterator it = v.begin(); it != v.end(); it++) {
		o << "v[] = " << *(it) << std::endl;
	}
	//std::cout  << "fin de print vector" << std::endl;
}

void	printMap(std::map<std::string, std::string> map, std::ostream & o);

#endif