#include "parse_http.hpp"
#include "Socket.hpp"
std::vector<std::string>	split_and_trim(std::string line);

Parse_http::Parse_http()
{
}

Parse_http::~Parse_http()
{
}

int Parse_http::HandleOneSocket(t_socket socket)
{
    if (recv(socket.clientFd, this->request, 4096, 0) == -1) {
		std::cerr << "Error bind : " << std::endl << strerror(errno) << std::endl;
        close(socket.listenFd);
        close(socket.clientFd);
        return 0;
    }
    if (this->GetData() == -1)
    {
        // mauvais format 400
        // 
        return 1;
    }
    this->PrintData();
    




    // on repond
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length: 1234\nConnection: keep-alive\nDate: Wed, 26 Sep 2024 12:00:00 GMT\n\n<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Bienvenue</title>\n</head>\n<body>\n<h1>Bienvenue sur notre site !</h1>\n<p>Ceci est la page d'accueil.</p>\n</body>\n</html>";
	const char *hello2 = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length: 1234\nConnection: keep-alive\nDate: Wed, 26 Sep 2024 12:00:00 GMT\n\n<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Bienvenue</title>\n</head>\n<body>\n<h1>Bienvenue sur notre site !</h1>\n<p>Ceci est la page d'accueil.</p>\n</body>\n</html>";
    send(socket.clientFd, hello2, strlen(hello), 0);// plutot utiliser send
    close(socket.clientFd);
	return 1;
}

int Parse_http::GetData()
{
    size_t start = 0;
	size_t end = 0;
    std::string str_request(std::string(this->request).substr(0, std::string(this->request).find("\n\n")));
    if (std::string(this->request).find("\n\n") != std::string::npos) {
        this->body = std::string(this->request).substr(std::string(this->request).find("\n\n"));
    }
	while (end != std::string::npos)// on prend le header et la ligne d etat ligne par ligne et on trime au cas ou
	{
		end = str_request.find("\r\n", start);
		if (end > start) {
			std::string str = str_request.substr(start, end - start);
			size_t first = str.find_first_not_of(" \t\n\r\f\v");
			size_t last = str.find_last_not_of(" \t\n\r\f\v");
			if (first != std::string::npos)
				this->header.push_back(str.substr(first, last - first + 1));
		}
		start = end + 1;
	}
    start = 0;
    end = this->header.front().find(' ', 0);
    if (end == std::string::npos)
        return 0;// 400 mauvais format
    this->methode = this->header.front().substr(start, end);
    start = end + 1;
    end = this->header.front().find(' ', start);
    if (end == std::string::npos)
        return 0;// 400 mauvais format
    this->ressource = this->header.front().substr(start, end - start);
    start = end + 1;
    if (this->header.front().substr(start) != "HTTP/1.1")
        return 0;// 400 mauvais format ou version, selon ce que l'on fait quand c'est pas 1.1 faudra ajuster
    this->http_version = 1.1;
    for (size_t i = 1; i < this->header.size(); ++i) {
        size_t colon = this->header[i].find(":");
        this->header_map[this->header[i].substr(0, colon)] = this->header[i].substr(colon + 2);
        std::cout << this->header[i].substr(0, colon) << std::endl << this->header[i].substr(colon + 2) << std::endl;
    }
    return 1;
}

void Parse_http::PrintData()
{
    std::cout << std::endl << RED << this->request << WHITE << std::endl;
    std::cout << BLUE;
    std::cout << "|" << this->methode << "|" << std::endl << "|" << this->ressource << "|" << std::endl << "|" << this->http_version << "|" << std::endl;
    std::cout << YELLOW;
    for (size_t i = 0; i < this->header.size(); ++i) {
        std::cout << this->header[i] << std::endl << std::endl;
    }
    std::cout << RED;
    std::cout << this->body;
    std::cout << WHITE;
}