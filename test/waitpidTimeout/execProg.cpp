#include <unistd.h> // fork
#include <string.h> // strerror
#include <iostream> // c++
#include <sys/wait.h> // waitpid
#include <sys/types.h> // kill
#include <sys/select.h> // select

int	main(int ac, char **av, char **envp)
{
	int waitpidStatus = 0;

	if (ac != 2)
	{
		std::cout << "only prog path needed" << std::endl;
		return 2;
	}
	char *args[] = {av[1], NULL};
	pid_t pid = fork();

	if (pid == 0) {
		std::cout << "enfant" << std::endl;
		sleep(5);
		execve(av[1], args, envp);
	}
	else
	{
		int count = 5;

		fd_set fds;
		struct timeval tv;
		
		FD_ZERO(&fds);
		FD_SET(0, &fds);

	
		while (count > 0) {
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			if (select(1, &fds, NULL, NULL, &tv) == -1) {
				std::cerr << "Error with select: " << strerror(errno) << std::endl;
			}
			if ((waitpidStatus = waitpid(pid, NULL, WNOHANG)) == 0) {
				std::cout << "still no pid that returned..." << std::endl;
			}
			if (waitpidStatus == -1) {
				std::cerr << "Error with waitpid " << strerror(errno) << std::endl;
				return 1;
			}
			else if (waitpidStatus > 0) {
				std::cout << "process termine" << std::endl;
				break ;
			}
			count--;
		}

		if (count == 0) {
			std::cout << "the process timed out" << std::endl;
			if (kill(pid, SIGKILL) == 0)
				std::cout << "process " << pid << " killed succesfully" << std::endl;
			else {
				std::cerr << "Error with kill: " << strerror(errno) << std::endl;
			}
		}
		std::cout << "parent" << std::endl;
	}
}