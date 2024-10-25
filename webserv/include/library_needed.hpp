#ifndef LIBRARY_HPP
#define LIBRARY_HPP

	#define RESET   "\033[0m"
	#define BLACK   "\033[0;30m"
	#define RED     "\033[0;31m"
	#define GREEN   "\033[0;32m"
	#define YELLOW  "\033[0;33m"
	#define BLUE    "\033[0;34m"
	#define MAGENTA "\033[0;35m"
	#define CYAN    "\033[0;36m"
	#define WHITE   "\033[0;37m"

	#include <sys/socket.h> // socket + network function
	#include <fcntl.h>
	#include <netinet/in.h> // netwok

	#include <iostream> // std
	#include <sstream> // type for file input
	#include <exception> // exception
	#include <stdint.h> // uint16_t
	#include <string> // 
	#include <algorithm> // std::find
	#include <cstring> // 
	#include <fstream> // inputfile
	#include <string> // string manip

	#include <map> // map
	#include <vector> // vector

	#include <unistd.h> // access
	#include <stdio.h> // perror
	#include <string.h> // strerror

	#include <limits.h> // limits value

	#include <sys/epoll.h> // epoll
	
#endif