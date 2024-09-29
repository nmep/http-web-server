#include "parse_http.hpp"

Parse_http::Parse_http(Server serv)
{
    this->serv = serv;
    this->code = 0;// juste une precaution

    this->mime[".html"] = "text/html";
    this->mime[".htm"] = "text/html";
    this->mime[".css"] = "text/css";
    this->mime[".ico"] = "image/x-icon";
    this->mime[".avi"] = "video/x-msvideo";
    this->mime[".bmp"] = "image/bmp";
    this->mime[".doc"] = "application/msword";
    this->mime[".gif"] = "image/gif";
    this->mime[".gz"] = "application/x-gzip";
    this->mime[".jpg"] = "image/jpeg";
    this->mime[".jpeg"] = "image/jpeg";
    this->mime[".png"] = "image/png";
    this->mime[".txt"] = "text/plain";
    this->mime[".mp3"] = "audio/mp3";
    this->mime[".pdf"] = "application/pdf";
        
    this->code_map[100] = "Continue";
    this->code_map[101] = "Switching Protocol";
    this->code_map[200] = "OK";
    this->code_map[201] = "Created";  
    this->code_map[202] = "Accepted";
    this->code_map[203] = "Non-Authoritative Information";
    this->code_map[204] = "No Content";
    this->code_map[205] = "Reset Content";
    this->code_map[206] = "Partial Content";
    this->code_map[300] = "Multiple Choice";
    this->code_map[301] = "Moved Permanently";
    this->code_map[302] = "Moved Temporarily";
    this->code_map[303] = "See Other";
    this->code_map[304] = "Not Modified";
    this->code_map[307] = "Temporary Redirect";
    this->code_map[308] = "Permanent Redirect";
    this->code_map[400] = "Bad Request";
    this->code_map[401] = "Unauthorized";
    this->code_map[403] = "Forbidden";
    this->code_map[404] = "Not Found";
    this->code_map[405] = "Method Not Allowed";
    this->code_map[406] = "Not Acceptable";
    this->code_map[407] = "Proxy Authentication Required";
    this->code_map[408] = "Request Timeout";
    this->code_map[409] = "Conflict";
    this->code_map[410] = "Gone";
    this->code_map[411] = "Length Required";
    this->code_map[412] = "Precondition Failed";
    this->code_map[413] = "Payload Too Large";
    this->code_map[414] = "URI Too Long";
    this->code_map[415] = "Unsupported Media Type";
    this->code_map[416] = "Requested Range Not Satisfiable";
    this->code_map[417] = "Expectation Failed";
    this->code_map[418] = "I'm a teapot";
    this->code_map[421] = "Misdirected Request";
    this->code_map[425] = "Too Early";
    this->code_map[426] = "Upgrade Required";
    this->code_map[428] = "Precondition Required";
    this->code_map[429] = "Too Many Requests";
    this->code_map[431] = "Request Header Fields Too Large";
    this->code_map[451] = "Unavailable for Legal Reasons";
    this->code_map[500] = "Internal Server Error";
    this->code_map[501] = "Not Implemented";
    this->code_map[502] = "Bad Gateway";
    this->code_map[503] = "Service Unavailable";
    this->code_map[504] = "Gateway Timeout";
    this->code_map[505] = "HTTP Version Not Supported";
    this->code_map[506] = "Variant Also Negotiates";
    this->code_map[507] = "Insufficient Storage";
    this->code_map[510] = "Not Extended";
    this->code_map[511] = "Network Authentication Required";
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
        return 1;
    this->GenerateAnswer();
    this->PrintData();




    // on repond
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length: 1234\nConnection: keep-alive\nDate: Wed, 26 Sep 2024 12:00:00 GMT\n\n<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Bienvenue</title>\n</head>\n<body>\n<h1>Bienvenue sur notre site !</h1>\n<p>Ceci est la page d'accueil.</p>\n</body>\n</html>";
	const char *hello2 = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length: 1234\nConnection: keep-alive\nDate: Wed, 26 Sep 2024 12:00:00 GMT\n\n<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Bienvenue</title>\n</head>\n<body>\n<h1>Bienvenue sur notre site !</h1>\n<p>Ceci est la page d'accueil.</p>\n</body>\n</html>";
    send(socket.clientFd, hello2, strlen(hello), 0);
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
    {
        this->code = 400;
        return 0;
    }
    this->methode = this->header.front().substr(start, end);
    start = end + 1;
    end = this->header.front().find(' ', start);
    if (end == std::string::npos)
    {
        this->code = 400;
        return 0;
    }
    this->ressource = this->header.front().substr(start, end - start);
    start = end + 1;
    if (this->header.front().substr(start) != "HTTP/1.1")
    {
        this->code = 400;// 400 mauvais format ou version, selon ce que l'on fait quand c'est pas 1.1 faudra ajuster
        return 0;
    }
    this->http_version = "HTTP/1.1";
    for (size_t i = 1; i < this->header.size(); ++i) {
        size_t colon = this->header[i].find(":");
        this->header_map[this->header[i].substr(0, colon)] = this->header[i].substr(colon + 2);
        std::cout << this->header[i].substr(0, colon) << std::endl << this->header[i].substr(colon + 2) << std::endl;
    }
    // verifier que la methode est reconnu et que la ressource soit accesible et exite
    this->code = 200;
    return 1;
}

void Parse_http::PrintData()
{
    std::cout << std::endl << RED << this->request << WHITE << std::endl;
    // std::cout << BLUE;
    // std::cout << "|" << this->methode << "|" << std::endl << "|" << this->ressource << "|" << std::endl << "|" << this->http_version << "|" << std::endl;
    // // std::cout << BLUE;
    // for (size_t i = 0; i < this->header.size(); ++i) {
    //     std::cout << this->header[i] << std::endl << std::endl;
    // }
    // std::cout << RED;
    // std::cout << this->body;
    std::cout << GREEN;
    std::cout << answer << std::endl;
    std::cout << WHITE;
}

std::string Parse_http::GetMime(std::string extension)
{
    if ( this->mime.find(extension) != this->mime.end())
        return (this->mime[extension]);
    return ("text/html");// si on trouve pas on affiche quand meme sout format html ou on gere pas ?
}

std::string Parse_http::GetCodeSentence(int code)
{
    if ( this->code_map.find(code) != this->code_map.end())
        return (this->code_map[code]);
    return ("Undefined");// c'est pas senser arriver
}

void Parse_http::GenerateAnswer()
{
    std::stringstream stream << // convertir le int et string
    const std::string c = std::to_string(this->code);
    this->answer.append(this->http_version + " " + c + "\r\n");// ne pas oublier de rajouter la phrase de raison todo
    this->contentType();
    this->connection();
    this->server();
    this->location();
    this->date();
    //taille

    this->answer.append("\r\n");
    // on remplit le body
}

void Parse_http::contentType()
{
    if(this->code != 200)
        return;
    this->answer.append("Content-Type: ");
    size_t dot = this->ressource.find('.');
    if (dot != std::string::npos && dot < this->ressource.size() && this->mime.find(this->ressource.substr(dot)) != this->mime.end())
        this->answer.append(this->GetMime(this->ressource.substr(dot)));
    else
        this->answer.append(this->GetMime("notfound"));
    this->answer.append("\r\n");
}

void Parse_http::connection()
{
    if(this->header_map.find("Connection") != this->header_map.end() && this->header_map["Connection"] == "keep-alive")
        this->answer.append("Connection: keep-alive\r\n");
}

void Parse_http::server()// faut que j'ai acces au nom du serveur
{
    this->answer.append("Servname:" + this->serv.GetServerName() + "\r\n");
}

void Parse_http::location()// on le met que pour 201 (post) et les redirections (300+)
{
    //pour post on creer un nouveau file sauf si il existe deja et on genere son nom aleatoirement cf uuid ou alors incrementation
}

void Parse_http::date()
{
    char date[1000];
    time_t now = time(0) + 7200;//7200 ca vaut 2h en secondes
    struct tm tm = *gmtime(&now);
    strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
    this->answer.append("Date: " + std::string(date) + "\r\n");
}