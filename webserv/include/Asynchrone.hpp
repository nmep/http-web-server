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
        Configuration conf;// les parametres du serveur web

    public:
        Asynchrone(Configuration const &conf, size_t nb_serv);
        ~Asynchrone();

        void Server_action(int server_idx, int socket_fd);
};

#endif