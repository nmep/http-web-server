#include <dirent.h>
#include <iostream>

int	main(int ac, char **av)
{
	if (ac != 2)
		return 2;
	DIR	*dir = opendir(av[1]);
	if (dir == NULL) {
		std::cout << "c'est pas un dossier" << std::endl;
		return 2;
	}
	else {
		std::cout << "c'est un dossier" << std::endl;
		closedir(dir);
		return 0;
	}
	return 1;
}