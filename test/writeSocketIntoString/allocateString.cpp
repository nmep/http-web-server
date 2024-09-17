#include <iostream>
#include <string.h>
#include <vector>
#include <unistd.h>

#define READ_SIZE 10

int	main()
{
	std::vector<char> buff;
	int lengthTot = 0;
	std::vector<char>::iterator it;
	// buff.resize(READ_SIZE);x
	while (1)
	{
		buff.resize(lengthTot + READ_SIZE);
		lengthTot += read(0, &buff[lengthTot], READ_SIZE);
		buff.resize(lengthTot);
		if (lengthTot)
		std::cout << lengthTot << " size v = " << buff.size() << std::endl;
		for (it = buff.begin(); it != buff.end(); it++) {
			std::cout << *it;
		}
		std::cout << std::endl;
	}
}