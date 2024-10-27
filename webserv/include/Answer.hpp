#ifndef ANSWER_HPP
#define ANSWER_HPP

#include "library_needed.hpp"
#include "configuration.hpp"
#include "Epoll.hpp"
#include "Server.hpp"
#include "autoIndex.hpp"
#include <sys/stat.h> // pour stat
#include <iostream> // les bools
#include <string>
#include <algorithm>
#include <fstream>
#include <signal.h> // pour kill
#include <wait.h>// pour wait
#include <sstream> //stringstream

#define READ_SIZE 40960 // pour l'instant choisi arbitrairement, on verra si on le change pour plus de performance
#define LIMIT_SIZE_BODY_SERVER 104857600000 //equivalent a 1 Mo, pour ne pas saturer la memoire vive
#define LIMIT_SIZE_BEFORE_BODY_SERVER 1048576111111
#define LIM_SIZE_READ_FILE 1048576111111

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
		// garfi

        std::string match_location;
		bool autoindex;
        int fd_read;// utiliser pour la ressource a lire ou le fichier d'erreure a lire aussi
        int fd_write;
        bool cgi;
		std::ifstream *read_file;// pourquoi, on a pas le droit de read et write sans les fonctions en c todo
        size_t nb_readfile;
        int cgi_pid;

		size_t before_body_len;
        std::string remaining_part;
        int step;// 0 pour la ligne d'etat
                 // 1 pour le header
                 // 2 pour le body
                 // 3 quand on veut se barrer de force (pour get et delete quand on arrive au \r\n\r\n)
        std::string piece_of_request;

        void find_ressource_path(Configuration const &conf);
        int is_that_a_directory();
        void find_good_index_or_autoindex(Configuration const &conf);
        bool isBinary();
        bool isScript();
        void HandleError(Configuration const &conf);
        void cgi_from_post();
        char** ft_build_env(Configuration const &conf, std::string extension);
		void write_for_cgi(Configuration const &conf);
		void build_actual_cgi_env_var();

		std::string request;
		std::string methode;
		std::string ressource;// la ressource tel que dans la requete
		std::string ressource_path;// le chemin de la ressource sur notre machine
		std::string cgi_env_var;//les variables donnee dans l'url pour le cgi
		std::map<std::string, std::string> header_map;// les elements du header associes a leur valeur
		std::string request_body;

		// upload file
		// 1 first line
		std::string beginBoundary;
		std::string endBoundary;
		// 2 nd line
		std::string fileName;
		// 3rd line mime type
		std::string mimeFile;
		std::string mimeStr;

		//
		// tools
		std::map<std::string, std::string> mime_map;
		std::map<int, std::string> code_map;

		std::map<std::string, std::string> carctere_special_map;
		std::string redirection;

        void first_step(size_t bytesRead);
        void second_step(size_t bytesRead);
        void third_step(size_t bytesRead);
        void parse_state_line(std::string state_line);
        void parse_header(std::string header);

        void GET(Configuration const &conf);
        void POST(Configuration const &conf, int server_idx);
        void DELETE(Configuration const &conf);
		std::string GetMime(std::string extansion);// prend l'extension du fichier en parametre et renvoie le type
		std::string GetCodeSentence(int code);// on renvoie la phrase de raison associe au code d etat
		void contentType();
		void connection();// uniquement quand c'est preciser keep alive dans la requete
		void server(Configuration const &conf);
		void location();// on precise on location dans les cas 201 et 3xx
		void date();
		void taille();
		void Reset();

		// upload file
		bool	parseBodyHeader();
		bool	parseBoundary(std::string line);
		bool	parseContentDisposition(std::string line);
		bool	parseFileName(std::string line);
		bool	parseContentType(std::string line);
		bool	uploadFile();
		inline bool	changeFileName(int FileNameIndex);
		inline void	randomName(int fileNameIndex);
		//

		Answer() {};// c'est toi qui l'as rajoute ?
    public:
		Answer(int server_idx);
        ~Answer();

		int GetSocketFd() const;
		void SetSocketFd(int fd);
        int GetStatus() const;
		void SetStatus(int status);

        void ReadRequest(Configuration const &conf, int socket_fd, int server_idx);
        void ReadFile(Configuration const &conf);
        void WriteFile(Configuration const &conf);
        void SendAnswer(Configuration const &conf);

};

#endif