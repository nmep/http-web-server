#ifndef SOCKET_HPP
	#define SOCKET_HPP

#include "library_needed.hpp"
#include "configuration.hpp"

/*	j'ai besoin d'une socket par serveur donc il me faut une structure par "serveur"
	pour gerer les differentes connections je vais avoir besoin dans tableau de structure.
			j'ai besoin du numero de fd associe et d'une string et la remplir de la requete

	socket:
		- un fd associe a la socket
	bind:
		- un structure sockaddr_in
*/

typedef	struct	s_socket
{
	int	socketFd;
	int	socketFlag;
	struct sockaddr_in	addr;
}				t_socket;


	class Socket
	{
		private:
			t_socket	*sockets;
		public:
			Socket();
			// Socket(Socket const & copy);
			~Socket();
			// Socket&	operator=(Socket const & rhs);
			int	initAllSockets(Configuration const & conf);
			int	initOneSocket(t_socket *socketStruct, Configuration const & conf, int index);
	};

#endif