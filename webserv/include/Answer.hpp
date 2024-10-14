#ifndef ANSWER_HPP
#define ANSWER_HPP

#include "library_needed.hpp"
#include "configuration.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include <sys/stat.h> // pour stat
#include <iostream> // les bools
#include <string>
#include <algorithm>
#include <fstream>
#include <signal.h> // pour kill
#include <wait.h>// pour wait


#define READ_SIZE 4096 // pour l'instant choisi arbitrairement, on verra si on le change pour plus de performance

class Answer
{
    private:
        int server_idx;
        int socket_fd;
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
        std::string ressource;// la ressource tel que dans la requete
        std::string ressource_path;// le chemin de la ressource sur notre machine
        std::string cgi_env_var;//les variables donnee dans l'url pour le cgi
        std::map<std::string, std::string> header_map;// les elements du header associes a leur valeur
        std::string request_body;

        // tools
        std::map<std::string, std::string> mime_map;
        std::map<int, std::string> code_map;

        bool autoindex; // faut que tu le mette dans la class serv pas dans location
        std::string match_location;
        int fd_read;// utiliser pour la ressource a lire ou le fichier d'erreure a lire aussi
        int fd_write;
        std::ifstream *read_file;
        bool cgi;
        size_t nb_readfile;
        int cgi_pid;

        void DoneWithRequest(Configuration const &conf);
        void ParseRequest();
        void find_ressource_path(Configuration const &conf);
        int is_that_a_directory();
        void find_good_index_or_autoindex(Configuration const &conf);
        bool isBinary();
        bool isScript();
        void HandleError(Configuration const &conf);
        void cgi_from_post();
        char** ft_build_env(Configuration const &conf);


        std::string GetMime(std::string extansion);// prend l'extension du fichier en parametre et renvoie le type
        std::string GetCodeSentence(int code);// on renvoie la phrase de raison associe au code d etat
        void contentType();
        void connection();// uniquement quand c'est preciser keep alive dans la requete
        void server(Configuration const &conf);
        void location();// on precise on location dans les cas 201 et 3xx
        void date();
        void taille();
        void Reset();

        void GET(Configuration const &conf);
        void POST(Configuration const &conf);
        void DELETE();

    public:
        Answer(int server_idx);
        ~Answer();

        int GetStatus() const;

        void ReadRequest(Configuration const &conf, int socket_fd);
        void ReadFile();
        void WriteFile(Configuration const &conf);
        void SendAnswer(Configuration const &conf);

};

#endif