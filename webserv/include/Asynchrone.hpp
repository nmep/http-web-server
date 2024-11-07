#ifndef ASYNCHRONE_HPP
#define ASYNCHRONE_HPP

#include "library_needed.hpp"
#include "configuration.hpp"
#include "Epoll.hpp"
#include "Server.hpp"
#include "Answer.hpp"

class Asynchrone
{
    private:

    public:
        Asynchrone(size_t nb_serv);
        ~Asynchrone();

        void Server_action(Configuration const &conf, int server_idx, int socket_fd, int servConfIdx, \
			int epfd, int nfd, struct epoll_event events[], t_socket *sockets, int portListeningLen);

        std::vector<Answer> Answers_instances;// de longeur nb_serv
                                              // regroupe toutes les instances de la class Answer
};

#endif