#include "Answer.hpp"

Answer::Answer(int server_idx)
{
    this->server_idx = server_idx;
    this->status = 0;
    this->code = 200;

    this->mime_map[".html"] = "text/html";
    this->mime_map[".htm"] = "text/html";
    this->mime_map[".css"] = "text/css";
    this->mime_map[".ico"] = "image/x-icon";
    this->mime_map[".avi"] = "video/x-msvideo";
    this->mime_map[".bmp"] = "image/bmp";
    this->mime_map[".doc"] = "application/msword";
    this->mime_map[".gif"] = "image/gif";
    this->mime_map[".gz"] = "application/x-gzip";
    this->mime_map[".jpg"] = "image/jpeg";
    this->mime_map[".jpeg"] = "image/jpeg";
    this->mime_map[".png"] = "image/png";
    this->mime_map[".txt"] = "text/plain";
    this->mime_map[".mp3"] = "audio/mp3";
    this->mime_map[".pdf"] = "application/pdf";

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

Answer::~Answer()
{
}

int Answer::GetStatus() const
{
    return this->status;
}

std::string Answer::GetMime(std::string extension)
{
    if (this->mime_map.find(extension) != this->mime_map.end())
        return (this->mime_map[extension]);
    return ("text/html");// si on trouve pas on affiche quand meme sout format html ou on gere pas ?
}

std::string Answer::GetCodeSentence(int code)
{
    if (this->code_map.find(code) != this->code_map.end())
        return (this->code_map[code]);
    return ("Undefined");// c'est pas senser arriver
}

// renvoie 1 si le vrai chemin de la ressource mene a un dossier, 0 si c'est un file et sinon 2 (si le path existe pas par example)
int Answer::is_that_a_directory()
{
    struct stat info;

    if (stat(this->ressource_path.c_str(), &info) == -1)
    {
        std::cerr << "Erreur lors de l'accès au chemin. " << this->ressource_path.c_str() << std::endl;
        // return 2;
    }
    if (S_ISREG(info.st_mode))
        return 0;//fichier
    else if (S_ISDIR(info.st_mode))
        return 1;//dossier
    return 2;// ni l'un ni l'autre, j'ai pas bien compris a quoi ca correspond
}

// on obtient l'emplacement de la ressource sur notre machine
// ex: pour une loc /blabla, un root theRoot et une requete /blabla/fichier.html -> on renvoit theRoot/fichier.html
void Answer::find_ressource_path(Configuration const &conf)
{
    size_t idx = 0;
    int depth = -1;
    std::map<std::string, Location*>::iterator it = conf.getServer(this->server_idx).getLocationMap().begin();
    while (idx++ < conf.getServer(this->server_idx).getLocationMap().size()) {
        this->autoindex = it->second->getAutoIndex();
        if (idx != 1)
            it++;
        if (it->first.size() <= this->ressource.size() && it->first == this->ressource.substr(0, it->first.size()))
        {
            int count = 0;
            for (size_t i = 0; i < it->first.size(); i++)
            {
                if (it->first[i] == '/')
                    count++;
            }
            if (it->first[it->first.size() - 1] == '/')
                count--;
            if (count > depth)
            {
                depth = count;
                if (it->first[it->first.size() - 1] == '/')
                    this->ressource_path = it->second->getRoot() + "/" + this->ressource.substr(it->first.size());
                else
                    this->ressource_path = it->second->getRoot() + this->ressource.substr(it->first.size());
                this->match_location = it->first;
            }
        }
        // std::cout << YELLOW << ressource << " " << this->match_location << " " << this->ressource_path << " " << this->ressource_path << WHITE << std::endl;
    }
    if (depth == -1)
    {
        this->ressource_path = this->ressource;
        this->match_location = "None";
        this->code = 404; 
        return ;
    }
    // on verifie que la methode soit bien autorisee
    bool find = false;
    std::vector<std::string> vec = conf.getServer(server_idx).getLocationMap()[match_location]->getAllowedMethodVector();
    for (std::vector<std::string>::iterator it = vec.begin() ;it != vec.end(); it++)
    {
        if (*it == this->methode)
        {
            find = true;
            break;
        }
    }
    if (find == false)
        this->code = 405;//methode not allowed
    return ;
}

void Answer::ParseRequest()
{
    size_t start = 0;
	size_t end = 0;
    std::string str_request(std::string(this->request).substr(0, std::string(this->request).find("\n\n")));
    if (std::string(this->request).find("\n\n") != std::string::npos)
        this->request_body = std::string(this->request).substr(std::string(this->request).find("\n\n"));
    std::vector<std::string> header;
	while (end != std::string::npos)// on prend le header et la ligne d etat ligne par ligne et on trime au cas ou
	{
		end = str_request.find("\r\n", start);
		if (end > start) {
			std::string str = str_request.substr(start, end - start);
			size_t first = str.find_first_not_of(" \t\n\r\f\v");
			size_t last = str.find_last_not_of(" \t\n\r\f\v");
			if (first != std::string::npos)
				header.push_back(str.substr(first, last - first + 1));
		}
		start = end + 1;
	}
    start = 0;
    end = header.front().find(' ', 0);
    if (end == std::string::npos)
    {
        this->code = 400;
        return ;
    }
    this->methode = header.front().substr(start, end);
    start = end + 1;
    end = header.front().find(' ', start);
    if (end == std::string::npos)
    {
        this->code = 400;
        return ;
    }
    this->ressource = header.front().substr(start, end - start);
    start = end + 1;
    if (header.front().substr(start) != "HTTP/1.1")
        this->code = 400;// 400 mauvais format ou version, selon ce que l'on fait quand c'est pas 1.1 faudra ajuster
    for (size_t i = 1; i < header.size(); ++i)
    {
        size_t colon = header[i].find(":");
        this->header_map[header[i].substr(0, colon)] = header[i].substr(colon + 2);
    }
}

// quand on an fini de lire la request, on la parse et on indique quelle sera la prochaine etape
void Answer::DoneWithRequest(Configuration const &conf)
{
    this->ParseRequest();
    this->find_ressource_path(conf);
    
    if (this->is_that_a_directory() == 1)
    {
        // this->find_good_index_or_autoindex();
        // pour l'instant je fait rien, on verra pour le vector index
        // a voir ou je prend auto index, si il est propre a chaque location ou si c'est le meme pour tout le server
    }
    this->autoindex = true;//temporaire TO DO enlever
    if (this->autoindex != true)// si il est encore active c'est qu'on a fait l'auto index et que j'ai deja un body, on peut envoyer
        this->status = 3;
    else if (this->code >= 400)
    {
        this->status = 1;
        // le code ne correspond a rien de nos page, a voir ce qu'on fait todo
    }
    else
    {
        this->status = 1;// attention, quand c'est pas get on gere differement
        if (access(this->ressource_path.c_str(), F_OK) == -1)
        {
            std::cout << "herre\n" << this->ressource_path;
            this->code = 404;//not found
            return ;
        }
        if (access(this->ressource_path.c_str(), F_OK | R_OK) == -1)
        {
            this->code = 403;//forbidden
            return ;
        }
        this->fd_read = open(this->ressource_path.c_str(), O_RDONLY);
        if (this->fd_read == -1)
        {
            std::cerr << "Erreur lors de l'ouverture du fichier" << std::endl;
            this->code = 500;// a voir quelle code on met quand le fichier ne s'ouvre pas  todo
            return ;
        }
    }
    return ;
}

// on lit la requete par tranche de READ_SIZE et quand on a fini on la parse avec DoneWithRequest
void Answer::ReadRequest(Configuration const &conf, int socket_fd)
{
    std::cout << RED << "Debut de ReadRequest" << WHITE << std::endl;

    char buffer[READ_SIZE];
    // ssize_t bytesRead = 1;
	// (void)socket_fd;
    ssize_t bytesRead = recv(socket_fd, buffer, READ_SIZE, 0);
    if (bytesRead == -1) {
		std::cerr << "Error with recv" << std::endl;// peut etre renvoyer une erreur cote client ou server
        close(socket_fd);
        return ;
    }

    buffer[bytesRead] = '\0';
    this->request.append(buffer);
	std::cout << "bytes read = " << bytesRead << " < " << READ_SIZE << " ?" << std::endl;
    if (bytesRead < READ_SIZE)
    {
        this->DoneWithRequest(conf);
        if (this->code >= 400)
        {
            this->status = 1;
            std::stringstream tmp;
            tmp << this->code;
            std::map<std::string, std::string> map = conf.getServer(this->server_idx).getErrorPageMap();
            if (map.find(tmp.str()) != map.end())
            {
                std::cout << map.size() << " et " <<  map[tmp.str()].c_str() << " et " << tmp.str() << std::endl;
                this->fd_read = open(map[tmp.str()].c_str(), R_OK);
                if (this->fd_read == -1)
                    this->code = 500;// peut etre
            }
            else
            {
                // si on a pas la page dans la config
                this->status = 3;
                std::stringstream tmp;
                tmp << this->code;
                this->answer_body = "<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Error " + tmp.str() + "</title>\n</head>\n<body>\n<h1>Error " + tmp.str() + "</h1>\n</body>\n</html>\n";
            }
        }
    }
    std::cout << this->code << " " << this->status << std::endl;
    if (bytesRead < READ_SIZE)// juste pour l'affichage
        std::cout << YELLOW << "Complete," << std::endl << this->request << WHITE;
    else
        std::cout << YELLOW << "Uncomplete," << WHITE << std::endl;
    std::cout << RED << "Fin de ReadRequest" << WHITE << std::endl;
}

// on lit le fichier demander, que ce soit la ressource ou un fichier d'erreur
void Answer::ReadFile()
{
    std::cout << RED << "Debut de ReadFile" << WHITE << std::endl;
    std::cout << this->code << " " << this->fd_read << std::endl;
    char buffer[READ_SIZE];
    int bytesRead;

    bytesRead = read(this->fd_read, buffer, READ_SIZE);
    if (bytesRead == -1) {
		std::cerr << "Error with read" << std::endl;// peut etre renvoyer une erreur cote client ou server
        close(this->fd_read);
        return ;
    }
    buffer[bytesRead] = '\0';
    this->answer_body.append(buffer);
    if (bytesRead < READ_SIZE)
    {
        close(this->fd_read);
        this->status = 3;
    }

    if (bytesRead < READ_SIZE)// juste pour l'affichage
        std::cout << YELLOW << "Complete," << std::endl << this->answer_body << WHITE << std::endl;
    else
        std::cout << YELLOW << "Uncomplete," << WHITE << std::endl;
    std::cout << RED << "Fin de ReadFile" << WHITE << std::endl;
}

void Answer::WriteFile()
{
    std::cout << RED << "Debut de WriteFile" << WHITE << std::endl;

    std::cout << RED << "Fin de WriteFile" << WHITE << std::endl;
}

void Answer::SendAnswer(Configuration const &conf, int socket_fd)
{
    std::cout << RED << "Debut de SendAnswer" << WHITE << std::endl;
    std::stringstream tmp;
    tmp << this->code;

    this->answer.append("HTTP/1.1 " + tmp.str() + " " + this->GetCodeSentence(this->code) + "\r\n");
    this->contentType();
    this->connection();
    this->server(conf);
    this->location();
    this->date();
    this->taille();
    this->answer.append("\r\n");

    // on remplit le body
    if (this->answer_body.size() != 0)
        this->answer.append(this->answer_body);

    send(socket_fd, this->answer.c_str(), strlen(this->answer.c_str()), 0);
    close(socket_fd);
    std::cout << YELLOW << this->answer << WHITE << std::endl;
    std::cout << RED << "Fin de SendAnswer" << WHITE << std::endl;
    this->Reset();
}

void Answer::contentType()
{
    if(this->code != 200)
        return;
    this->answer.append("Content-Type: ");
    size_t dot = this->ressource_path.find('.');
    if (dot != std::string::npos && dot < this->ressource_path.size() && this->mime_map.find(this->ressource_path.substr(dot)) != this->mime_map.end())
        this->answer.append(this->GetMime(this->ressource_path.substr(dot)));
    else
        this->answer.append(this->GetMime("notfound"));
    this->answer.append("\r\n");
}

void Answer::connection()
{
    if(this->header_map.find("Connection") != this->header_map.end() && this->header_map["Connection"] == "keep-alive")
        this->answer.append("Connection: keep-alive\r\n");
}

void Answer::server(Configuration const &conf)
{
    this->answer.append("Server: " + conf.getServer(server_idx).GetServerName() + "\r\n");
}

void Answer::location()// on le met que pour 201 (post) et les redirections (300+)
{
    //pour post on creer un nouveau file sauf si il existe deja et on genere son nom aleatoirement cf uuid ou alors incrementation
}

void Answer::date()
{
    char date[1000];
    time_t now = time(0) + 7200;//7200 ca vaut 2h en secondes
    struct tm tm = *gmtime(&now);
    strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
    this->answer.append("Date: " + std::string(date) + "\r\n");
}

void Answer::taille()
{
    if (this->answer_body.size() > 0)// peut etre initialiser le answerbody a "" au debut
    {
        std::stringstream tmp;
        tmp << this->answer_body.size();
        this->answer.append("Content-Length: " + tmp.str() + "\r\n");
    }
}

void Answer::Reset()
{
    this->status = 0;
    this->code = 200;
    this->answer.clear();
    this->answer_body.clear();

    this->request.clear();
    this->methode.clear();
    this->ressource_path.clear();
    this->header_map.clear();
    this->request_body.clear();

    // peut etre qu'on reset aussi l'auto index
    this->match_location.clear();
}