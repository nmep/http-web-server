#include <iostream>
#include <cstring>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>

int	main(int ac, char **av)
{

	std::string line = std::string("Content-Disposition: form-data; name=\"file\"; filename=") + av[1];
	std::string name;

	if (line.find("filename=\"") == -1) {
		std::cout << "filename n'a pas de quote ou error avec file name" << std::endl;
		return 2;
	}
	std::cout << line << std::endl;
	name = line.substr(line.find("filename=\"") + std::strlen("filename=\""), line.size());
	std::cout << name << std::endl;

	if (name.find_last_of('"') == -1) {
		std::cerr << "il n'y a pas de quote fermante" << std::endl;
		return 2;
	}
	std::cout << "name size apres = " << name.size() << std::endl;
	std::cout << "name lat of \" = " << (name.find_last_of('"')) << std::endl;

	name = name.substr(0,name.find_last_of('"'));
	std::cout << '[' << name << ']' << std::endl;
	
	if (name.empty()) {
		std::cout << "file name est vide" << std::endl;
		int randNameFD = open("/dev/urandom", O_RDONLY);
		if (randNameFD == -1) {
			std::cerr << "Error with file /dev/urandom " << strerror(errno) << std::endl;
			return 2;
		}
		char randBuffName[10]; // il me faut le type de fichier ici qui est indiquier a la ligne suivant du body header donc je doit en premier lui tout parse puis faire la creation du fichier
		read(randNameFD, randBuffName, 9);
		randBuffName[9] = 0;
		std::cout << "randbuffName = " << randBuffName << std::endl;
	}

		
	return 0;
}