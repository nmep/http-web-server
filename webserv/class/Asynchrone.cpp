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
void Asynchrone::Server_action(Configuration const &conf, int server_idx, int socket_fd, int servConfIdx,
		int epfd, int nfd, struct epoll_event events[], t_socket *sockets, int portListeningLen)
{
	(void)servConfIdx; /* index de la configuration du serveur*/
    //std::cout  << BLUE << "Debut de Server_action, serv numero " << server_idx << ", socket_fd " << socket_fd << RESET << std::endl;
	//std::cout  << RED << "Server conf index = " << servConfIdx << RESET << std::endl;
	// //std::cout  << conf.getServer(servConfIdx) << std::endl;
	//std::cout  << "Upload store du serveur demande = " << conf.getServer(servConfIdx).getUploadStore() << std::endl;
	// sleep(5);
    switch (this->Answers_instances[server_idx].GetStatus())
    {
        case 0:
            this->Answers_instances[server_idx].ReadRequest(conf, socket_fd, servConfIdx);
            break;
        case 1:
            this->Answers_instances[server_idx].ReadFile(conf);
            break;
        case 2:
            this->Answers_instances[server_idx].WriteFile(conf, servConfIdx, \
				epfd, nfd, events, sockets, portListeningLen);
            break;
        case 3:
            this->Answers_instances[server_idx].SendAnswer(conf);
            break;
    }
    //std::cout  << BLUE << "Fin de Server_action, serv numero " << server_idx << ", nouveau status " << this->Answers_instances[server_idx].GetStatus() << WHITE << std::endl;
}