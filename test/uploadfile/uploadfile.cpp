#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <string>

#define BUFFER_SIZE 4096

int	main(int ac, char **av)
{
	std::ifstream inputFile(av[1], std::ios::binary);

	std::string str;
	char buffer[BUFFER_SIZE];

	while (inputFile.read(buffer, BUFFER_SIZE)) {
		// buffer[BUFFER_SIZE ] = '\0';
		str.append(buffer, BUFFER_SIZE);
	}

	std::cout << str << std::endl;
	std::ofstream outputFile("newImage.jpg", std::ios::binary);

	if (outputFile.is_open())
		std::cout << "le fichier est ouvert" << std::endl;
	outputFile.write(str.c_str(), str.size());
}