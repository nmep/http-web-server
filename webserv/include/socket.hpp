#ifndef SOCKET_HPP
	#define SOCKET_HPP

#include "library_needed.hpp"

/*	pour gerer les differentes connections je vais avoir besoin dans tableau de structure.
				j'ai besoin du numero de fd associe et d'une string et la remplir de la requete*/

typedef	struct	s_socket
{
	/* data */
	int fd;
	
}				t_socket;


	class Socket
	{
		private:
			
		public:
			Socket();
			Socket(Socket const & copy);
			~Socket();
			Socket&	operator=(Socket const & copy);
	};

#endif