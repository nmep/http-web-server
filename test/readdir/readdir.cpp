#include <dirent.h>
#include <stdlib.h>
#include <iostream>

int	main()
{
	DIR *dir;
	struct dirent *readDir;

	dir = opendir("/");
	if (dir == NULL)
		return 2;

	while ((readDir = readdir(dir)) != NULL) {
		std::cout << readDir->d_name << std::endl;
	}

	closedir(dir);
}