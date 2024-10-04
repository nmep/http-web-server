#ifndef ANSWER_HPP
#define ANSWER_HPP

#include "library_needed.hpp"
#include "configuration.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include <sys/stat.h> // pour stat
#include <iostream> // les bools


#define READ_SIZE 1024 // pour l'instant choisi arbitrairement, on verra si on le change pour plus de performance

class Answer
{
    private:
        int server_idx;
        int status; // de longeur nb_serv,
                    // 0 pour en cours de lecture de la requete,
                    // 1 pour en cours de lecture d'un file,
                    // 2 pour en cours d'ecriture d'un file,
                    // 3 pour en cours d'envoie de la reponse
                    // 4 pour en attente de reset
        
        // elements pour la response
        int code;
        std::string answer;
        std::string answer_body;

        // elements de la requete
        std::string request;
        std::string methode;
        std::string ressource_path;// le chemin de la ressource sur notre machine
        std::map<std::string, std::string> header_map;// les elements du header associes a leur valeur
        std::string request_body;

        // tools
        std::map<std::string, std::string> mime_map;
        std::map<int, std::string> code_map;

        bool autoindex; // faut que tu le mette dans la class serv pas dans location
        std::string match_location;
        int fd;// utiliser pour la ressource a lire ou le fichier d'erreures

        void DoneWithRequest(Configuration const &conf);
        void find_ressource_path(Configuration const &conf, std::string ressource);
        int is_that_a_directory();

        std::string GetMime(std::string extansion);// prend l'extension du fichier en parametre et renvoie le type
        std::string GetCodeSentence(int code);// on renvoie la phrase de raison associe au code d etat
        void contentType();
        void connection();// uniquement quand c'est preciser keep alive dans la requete
        void location();// on precise on location dans les cas 201 et 3xx
        void date();
        void taille();

    public:
        Answer(int server_idx);
        ~Answer();

        int GetStatus() const;

        void ReadRequest(Configuration const &conf, int socket_fd);
        void ReadFile();
        void WriteFile();
        void SendAnswer(Configuration const &conf, int socket_fd);

};

#endif