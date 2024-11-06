#include <iostream>
#include <vector>

int	main(int ac, char **av)
{
	if (ac != 3)
		return 2;
	
	std::vector<std::pair<std::string, std::string> > vp;

	for (int i = 0; i < 3; i++) {
		vp.push_back(std::make_pair(av[1], av[2]));
	}
	std::vector<std::pair<std::string, std::string> >::iterator it = vp.begin();

	std::cout << "vp size = " << vp.size() << "\n" << std::endl;
	for (/**/;it != vp.end(); it++) {
		std::cout << "first = " << it->first << std::endl;
		std::cout << "second = " << it->second << std::endl;
	}
}