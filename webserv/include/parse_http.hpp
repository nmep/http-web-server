#ifndef PARSE_HTTP_HPP
#define PARSE_HTTP_HPP

#include "library_needed.hpp"
#include "configuration.hpp"
#include "Socket.hpp"

class Parse_http
{
    private:
        char request[4096];// quel taille choisir ?
        std::vector<std::string> line;
        // char answer;
    public:
        Parse_http();
        ~Parse_http();

        int HandleOneSocket(t_socket socket);
        int GetData();
        void PrintData();

        
};

#endif