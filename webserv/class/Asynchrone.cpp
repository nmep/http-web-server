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
    std::cout << BLUE << "Debut de Server_action, serv numero " << server_idx << ", socket_fd " << socket_fd << std::endl;
	// std::cout << "case " << this->Answers_instances[server_idx].GetStatus() << std::endl;
	// sleep(1);
	// // server_idx = 105;
	if (server_idx == -1)// pour un test ??
		return ;
    switch (this->Answers_instances[server_idx].GetStatus())
    {
        case 0:
            this->Answers_instances[server_idx].ReadRequest(conf, socket_fd, server_idx);
            break;
        case 1:
            this->Answers_instances[server_idx].ReadFile();
            break;
        case 2:
            this->Answers_instances[server_idx].WriteFile();
            break;
        case 3:
            this->Answers_instances[server_idx].SendAnswer(conf);
            break;
    }
    std::cout << BLUE << "Fin de Server_action, serv numero " << server_idx << ", nouveau status " << this->Answers_instances[server_idx].GetStatus() << WHITE << std::endl;
    // si le status est mit a 4 apres ca, ca veut dire qu'on a fini avec cette reponse et qu'on doit la reset
}