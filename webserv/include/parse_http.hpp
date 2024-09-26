#ifndef PARSE_HTTP_HPP
#define PARSE_HTTP_HPP

#include "library_needed.hpp"
#include "configuration.hpp"
#include "Socket.hpp"

class Parse_http
{
    private:
        char request[4096];// quelle taille choisir ?
        std::string methode;
        std::string ressource;
        float http_version;
        std::vector<std::string> header;// le header ligne par ligne
        std::string body;
        std::map<std::string, std::string> header_map;
    public:
        Parse_http();
        ~Parse_http();

        int HandleOneSocket(t_socket socket);
        int GetData();
        void PrintData();
};

#endif