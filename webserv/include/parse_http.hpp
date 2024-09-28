#ifndef PARSE_HTTP_HPP
#define PARSE_HTTP_HPP

#include "library_needed.hpp"
#include "configuration.hpp"
#include "Socket.hpp"

class Parse_http
{
    private:
        char request[4096];// quelle taille choisir ?
        std::vector<std::string> header;// le header ligne par ligne avec la ligne d'etat
        std::string methode;
        std::string ressource;
        std::string http_version;
        std::map<std::string, std::string> header_map;
        std::string body;

        std::string answer;
        std::string code_and_sentence;// le code d'etat et la phrase de raison que l'on va renvoyer
        size_t body_length;
        std::string answer_body;

        std::map<std::string, std::string> mime;
        std::string servname;
        std::string loc;

        void contentType();
        void connection();
        void server();
        void location();
        void date();
    public:
        Parse_http();
        ~Parse_http();

        int HandleOneSocket(t_socket socket);
        int GetData();
        void PrintData();
        void GenerateAnswer();

        std::string GetMime(std::string extansion);
};

#endif