#ifndef	EPOLL_HPP
	#define EPOLL_HPP

	#define KAA 1
	#define MAX_EVENTS 4096

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
		int serverIdx;
		int	socketFlag; // fcntl pour que la socket soit non bloquante
		socklen_t addrLen;
		struct sockaddr_in	addr;
	}				t_socket;

		class Epoll
		{
			private:
				int					epfd; // tableau de int  chaque case represente une instance epoll par serveur
				int					nfd; // valeur de retour de epoll wait pour chaque serveur de nombre de fd pret a etre ecrit 
				t_socket			*sockets;
				int					*portListening; // tableau qui definit les port qui doivent etre mit sur ecoute en evitant les doublons
				int					portListeningLen; // taille de portListening
				std::map<int, int>	fdAndServer; // first fd de requete, second index du serveur associe a la requete
				std::map<int, int>	fdAndServerConfIdx;

			public:
				Epoll();
				// Epoll(Epoll const & copy);
				~Epoll();
				// Epoll&	operator=(Epoll const & rhs);
				int		initAllSockets(Configuration const & conf);
				int		initOneSocket(t_socket *socketStruct, int port);
				int		launchEpoll(Configuration const & conf);
				int		accept_and_save_connexion(int servId, int sockFD);
				int		setNonBlockSocket(int socket);
				int		isAnServerFd(int fd);
				bool	closeConnexion(int fd);

				int		getFdAndServer(int fd);
		};

#endif