#ifndef ANSWER_HPP
#define ANSWER_HPP

#include "library_needed.hpp"
#include "configuration.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include <sys/stat.h> // pour stat
#include <iostream> // les bools

// on cree cette classe a chaque fois que l'on veut repondre a une requete
// la fonction principal (handleonesocket) prend la socket en parametre, la lit, genere une reponse, et l'envoie. 
// Apres ca plus besoin de cette instance de la classe. On en creera une nouvelle des que la fonction launchEpoll (epoll.cpp) nous indique une nouvelle socket a gerer
class Answer
{
    private:
        int status;             // de longeur nb_serv,              // peut etre le mettre dans la class Answer directement
                                // 0 pour un nouveau,
                                // 1 pour en cours de lecture de la requete,
                                // 2 pour en cours de lecture d'un file demande,
                                // 3 pour en cours de lecture d'un file d'erreur
                                // 4 pour en cours d'ecriture du body
                                // 5 pour termine
        // elements de la requete
        char request[4096];// quelle taille choisir ?
        std::vector<std::string> header;// le header ligne par ligne + la ligne d'etat
        std::string methode;
        std::string ressource;
        std::string actual_path_ressource;// le chemin de la ressource sur notre machine
        std::string http_version;
        std::map<std::string, std::string> header_map;// juste chaque element du header de la requete avec sa "valeur"
        std::string body;// le body de la requete

        // elements de la reponse
        std::string answer;// la string dans laquel on ecrit la reponse
        int code;// le code d'etat que l'on renvoie
        size_t body_length;//
        std::string answer_body;
        std::string redir_loc; // la loc que l'on renvoie quand necessaire (201 et 3xx)

        // des trucs utiles
        Server serv;// la class serv pour avoir toutes les info utiles de la config (servname autoindex error page...)
        std::map<std::string, std::string> mime;// les extensions associees au type ex: .html avec text/html
        std::map<int, std::string> code_map;
        std::string match_location;// la location du fichier de config avec laquelle ca correpond
        bool autoindex;

        int GetData();// on recupere les donnees de la requete et on verifie que tout soit au bon format

        // pour construire le header de la reponse
        void GenerateAnswer();
        std::string GetMime(std::string extansion);// prend l'extension du fichier en parametre et renvoie le type
        std::string GetCodeSentence(int code);// on renvoie la phrase de raison associe au code d etat
        void contentType();
        void connection();// uniquement quand c'est preciser keep alive dans la requete
        void server();
        void location();// on precise on location dans les cas 201 et 3xx
        void date();
        void taille();

        void ressource_path();
        int is_that_a_directory();
        void find_good_index_or_autoindex();
        void read_file(std::string file);

        void get_content_error_page();
        // utiles
        void PrintData();

    public:
        Answer(Server &serv);
        ~Answer();

        int GetStatus() const;

        int HandleOneSocket(int fd);

};

#endif