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

    public:
        Asynchrone(size_t nb_serv);
        ~Asynchrone();

        void Server_action(Configuration const &conf, int server_idx, int socket_fd);
        
        std::vector<Answer> Answers_instances;// de longeur nb_serv
                                              // regroupe toutes les instances de la class Answer
                                              // doit etre en private mais la c'est temporaire todo
};

#endif