#ifndef ASYNCHRONE_HPP
#define ASYNCHRONE_HPP

#include "library_needed.hpp"
#include "configuration.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "Answer.hpp"

class Asynchrone
{
    private:
        std::vector<Answer> Answers_instances;// de longeur nb_serv
                                              // regroupe toutes les instances de la class Answer

    public:
        Asynchrone(size_t nb_serv);
        ~Asynchrone();

        void Server_action(Configuration const &conf, int server_idx, int socket_fd);
};

#endif