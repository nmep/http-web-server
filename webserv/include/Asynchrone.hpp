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
        std::vector<int> status;// de longeur nb_serv,              // peut etre le mettre dans la class Answer directement
                                // 0 pour un nouveau,
                                // 1 pour en cours de lecture de la requete,
                                // 2 pour en cours de lecture d'un file demande,
                                // 3 pour en cours de lecture d'un file d'erreur
                                // 4 pour en cours d'ecriture du body
        std::vector<Answer> Answers_instances;// de longeur nb_serv
                                              // regroupe toutes les instances de la class Answer
        Configuration conf;// les parametres du serveur web

    public:
        Asynchrone(Configuration &conf, size_t nb_serv);
        ~Asynchrone();

        void Server_action(int server_idx);
};

#endif