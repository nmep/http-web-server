#include "Asynchrone.hpp"

Asynchrone::Asynchrone(Configuration &conf, size_t nb_serv)
{
    this->conf = conf;
    for (size_t i = 0; i < nb_serv; i++)
        this->Answers_instances.push_back(Answer(this->conf.getServer(i)));
}

Asynchrone::~Asynchrone()
{
}

// on agit en fonction de l'avancee de la reponse concernant ce server
void Asynchrone::Server_action(int server_idx)
{
    switch (this->Answers_instances[server_idx].GetStatus())
    {
        case 0:
            this->Answers_instances[server_idx].FirstUse();
            break;
        case 1:
            this->Answers_instances[server_idx].ReadRequest();
            break;
        case 2:
            this->Answers_instances[server_idx].ReadAskedFile();
            break;
        case 3:
            this->Answers_instances[server_idx].ReadErrorFile();
            break;
        case 4:
            this->Answers_instances[server_idx].WriteAnswer();
            break;
    }
    // si le status est mit a 0 apres ca, ca veut dire qu'on a fini avec cette reponse et qu'on doit peut etre le reset
}