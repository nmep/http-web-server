#include "Answer.hpp"

Answer::Answer(Server &serv)
{
    this->serv = serv;
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
    if ( this->mime_map.find(extension) != this->mime_map.end())
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

    if (stat(this->ressource_path.c_str(), &info) != 0)
    {
        std::cerr << "Erreur lors de l'accès au chemin." << std::endl;
        return 2;
    }
    if (S_ISREG(info.st_mode))
        return 0;//fichier
    else if (S_ISDIR(info.st_mode))
        return 1;//dossier
    return 2;// ni l'un ni l'autre, j'ai pas bien compris a quoi ca correspond
}

// on obtient l'emplacement de la ressource sur notre machine
// ex: pour une loc /blabla, un root theRoot et une requete /blabla/fichier.html -> on renvoit theRoot/fichier.html
void Answer::find_ressource_path(std::string ressource)
{
    size_t idx = 0;
    int depth = -1;
    std::map<std::string, Location*>::iterator it = this->serv.getLocationMap().begin();
    while (idx++ < this->serv.getLocationMap().size()) {
        this->autoindex = it->second->getAutoInex();
        if (idx != 1)
            it++;
        if (it->first.size() <= ressource.size() && it->first == ressource.substr(0, it->first.size()))
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
                if (ressource[0] == '/')
                    this->ressource_path = it->second->getRoot() + this->ressource_path.substr(it->first.size() - 1);
                // else
                //     this->actual_path_ressource = it->second->getRoot() + "/" + this->ressource.substr(it->first.size());
                this->match_location = it->first;
            }
        }
        std::cout << YELLOW << this->ressource_path << " " << this->ressource_path << WHITE << std::endl;
    }
    if (depth == -1)
    {
        this->ressource_path = ressource;
        this->match_location = "None";
    }
    return ;
}

// quand on an fini de lire la request, on la parse et on indique quelle sera la prochaine etape
void Answer::DoneWithRequest()
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
    std::string ressource = header.front().substr(start, end - start);
    start = end + 1;
    if (header.front().substr(start) != "HTTP/1.1")
        this->code = 400;// 400 mauvais format ou version, selon ce que l'on fait quand c'est pas 1.1 faudra ajuster
    for (size_t i = 1; i < header.size(); ++i)
    {
        size_t colon = header[i].find(":");
        this->header_map[header[i].substr(0, colon)] = header[i].substr(colon + 2);
        std::cout << header[i].substr(0, colon) << std::endl << header[i].substr(colon + 2) << std::endl;
    }
    // verifier que la methode est reconnu todo
    this->find_ressource_path(ressource);
    if (this->is_that_a_directory() == 1)
    {
        // this->find_good_index_or_autoindex();
        // pour l'instant je fait rien, on verra pour le vector index
        this->autoindex = false;//temporaire
    }
    if (this->autoindex != true)// si il est encore active c'est qu'on a fait l'auto index et que j'ai deja un body
        this->status = 2;
    else if (this->code >= 400)
    {
        this->status = 1;
        std::stringstream tmp;
        tmp << this->code;
        for (std::map<std::string, std::string>::iterator it = this->serv.getErrorPageMap().begin(); it != this->serv.getErrorPageMap().end(); it++)
        {
            if (tmp.str() == it->first)
            {
                this->fd == open(it->second.c_str(), O_RDONLY);
                if (this->fd == -1)
                {
                    std::cerr << "Erreur lors de l'ouverture du fichier" << std::endl;
                    this->code = 500;// a voir quelle code on met quand le fichier ne s'ouvre pas  todo
                    return ;
                }
                return ;
            }
	}
    // le code ne correspond a rien de nos page, a voir ce qu'on fait todo
    }
    else
    {
        this->status = 1;// attention, quand c'est pas get on gere differement
        if (access(this->ressource_path.c_str(), F_OK) == -1)
        {
            this->code = 404;//not found
            return ;
        }
        if (access(this->ressource_path.c_str(), F_OK | R_OK) == -1)
        {
            this->code = 403;//forbidden
            return ;
        }
        this->fd = open(this->ressource_path.c_str(), O_RDONLY);
        if (this->fd == -1)
        {
            std::cerr << "Erreur lors de l'ouverture du fichier" << std::endl;
            this->code = 500;// a voir quelle code on met quand le fichier ne s'ouvre pas  todo
            return ;
        }
    }
    return ;
}

// on lit la requete par tranche de READ_SIZE et quand on a fini on la parse avec DoneWithRequest
void Answer::ReadRequest(int socket_fd)
{
    std::cout << "j'entre dans ReadRequest" << std::endl;

    char buffer[READ_SIZE];
    ssize_t bytesRead = recv(socket_fd, buffer, READ_SIZE, 0);
    if (bytesRead == -1) {
		std::cerr << "Error with recv" << std::endl;// peut etre renvoyer une erreur cote client ou server
        close(socket_fd);
        return ;
    }
    buffer[bytesRead] = '\0';
    this->request.append(buffer);
    if (bytesRead < READ_SIZE)
        this->DoneWithRequest();
    std::cout << "je sort de ReadRequest" << std::endl;
}

// on lit le fichier demander, que ce soit la ressource ou un fichier d'erreur
void Answer::ReadAskedFile()
{
    char buffer[READ_SIZE];
    size_t bytesRead;

    bytesRead = read(fd, buffer, READ_SIZE);
    if (bytesRead == -1) {
		std::cerr << "Error with read" << std::endl;// peut etre renvoyer une erreur cote client ou server
        close(this->fd);
        return ;
    }
    buffer[bytesRead] = '\0';
    this->answer_body.append(buffer);
    if (bytesRead < READ_SIZE)
    {
        close(fd);
        this->status = 2;
    }
}


void Answer::WriteAnswer(int socket_fd)
{
    std::stringstream tmp;
    tmp << this->code;

    this->answer.append("HTTP/1.1 " + tmp.str() + " " + this->GetCodeSentence(this->code) + "\r\n");
    this->contentType();
    this->connection();
    this->server();
    this->location();
    this->date();
    this->taille();
    this->answer.append("\r\n");
    // on remplit le body
    if (this->answer_body.size() != 0)
        this->answer.append(this->answer_body);

    // on send todo
    // on reset todo
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

void Answer::server()
{
    this->answer.append("Server: " + this->serv.GetServerName() + "\r\n");
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