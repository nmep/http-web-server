#include "Asynchrone.hpp"

Asynchrone::Asynchrone(size_t nb_serv)
{
    for (size_t i = 0; i < nb_serv; i++)
        this->Answers_instances.push_back(Answer(i));
}

Asynchrone::~Asynchrone()
{
}

// on agit en fonction de l'avancee de la reponse concernant ce server
void Asynchrone::Server_action(Configuration const &conf, int server_idx, int socket_fd)
{
    std::cout << BLUE << "Debut de Server_action" << WHITE << std::endl;
    switch (this->Answers_instances[server_idx].GetStatus())
    {
        case 0:
            this->Answers_instances[server_idx].ReadRequest(conf, socket_fd);
            break;
        case 1:
            this->Answers_instances[server_idx].ReadFile();
            break;
        case 2:
            this->Answers_instances[server_idx].WriteFile();
            break;
        case 3:
            this->Answers_instances[server_idx].SendAnswer(conf, socket_fd);
            break;
    }
    std::cout << BLUE << "Fin de Server_action" << WHITE << std::endl;
    // si le status est mit a 4 apres ca, ca veut dire qu'on a fini avec cette reponse et qu'on doit la reset
}