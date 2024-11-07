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
}