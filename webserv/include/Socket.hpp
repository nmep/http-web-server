#ifndef SOCKET_HPP
	#define SOCKET_HPP

#define KAA 1
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
	int	listenFd;
	int	clientFd;
	int	socketFlag; // fcntl pour que la socket soit non bloquante
	socklen_t addrLen;
	struct sockaddr_in	addr;
}				t_socket;

	class Socket
	{
		private:
			t_socket	*sockets;
			int			*portListening;
			int			portListeningLen;
		public:
			Socket();
			// Socket(Socket const & copy);
			~Socket();
			// Socket&	operator=(Socket const & rhs);
			int	initAllSockets(Configuration const & conf);
			int	initOneSocket(t_socket *socketStruct, int port);
			int	launchEpoll();
	};

#endif