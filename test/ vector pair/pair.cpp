#include <iostream>

int	main(int ac, char **av)
{
	std::pair<std::string, std::string> p(av[1], av[2]);

	std::cout << "first = " << p.first << " second = " << p.second << std::endl;

}