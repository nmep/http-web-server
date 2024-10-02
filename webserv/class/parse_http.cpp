#include "parse_http.hpp"

Parse_http::Parse_http(Server &serv)
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

int glob_idx = 0;
int Parse_http::HandleOneSocket(int fd)
{
    std::cout << "je demarre le traitement de la requete\n";
    if (recv(fd, this->request, 4096, 0) == -1) {
		std::cerr << "Error bind : " << std::endl << strerror(errno) << std::endl;
        close(fd);
        return 0;
    }
    if (this->GetData() == -1)
        return 1;
    this->GenerateAnswer();
    this->PrintData();



    send(fd, this->answer.c_str(), strlen(this->answer.c_str()), 0);
    glob_idx++;
    close(fd);
    std::cout << "j'ai fini avec cette requete\n";
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
    this->ressource_path();
    std::cout << "le path: " << this->actual_path_ressource << std::endl;
    if (this->is_that_a_directory() == 1)
    {
        // this->find_good_index_or_autoindex();
        // pour l'instant je fait rien, on verra pour le vector index
        this->autoindex = false;//temporaire
    }
    if (this->autoindex != true)// si il est encore active c'est qu'on a fait l'auto index et que j'ai deja un body
    {
        this->read_file();
    }
    std::stringstream tmp;// juste pour convertir un int en string
    tmp << this->code;

    this->answer.append(this->http_version + " " + tmp.str() + " " + this->GetCodeSentence(this->code) + "\r\n");
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

void Parse_http::server()
{
    this->answer.append("Server: " + this->serv.GetServerName() + "\r\n");
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

void Parse_http::taille()
{
    if (this->answer_body.size() > 0)// peut etre initialiser le answerbody a "" au debut
    {
        std::stringstream tmp;
        tmp << this->answer_body.size();
        this->answer.append("Content-Length: " + tmp.str() + "\r\n");
    }
}

void Parse_http::ressource_path()
{
    size_t idx = 0;
    int depth = -1;
    std::map<std::string, Location*>::iterator it = this->serv.getLocationMap().begin();
    while (idx++ < this->serv.getLocationMap().size()){
        this->autoindex = it->second->getAutoInex();
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
                if (this->ressource[0] == '/')
                    this->actual_path_ressource = it->second->getRoot() + this->ressource.substr(it->first.size() - 1);
                // else
                //     this->actual_path_ressource = it->second->getRoot() + "/" + this->ressource.substr(it->first.size());
                this->match_location = it->first;
            }
        }
        std::cout << YELLOW << this->actual_path_ressource << " " << this->ressource << WHITE << std::endl;
    }
    if (depth == -1)
    {
        this->actual_path_ressource = this->ressource;
        this->match_location = "None";
    }
    return ;
}

int Parse_http::is_that_a_directory()
{
    struct stat info;

    if (stat(this->actual_path_ressource.c_str(), &info) != 0)
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

void Parse_http::find_good_index_or_autoindex()
{
    int end_slash = 0;
    std::string fake_path = this->actual_path_ressource;// une copy du path avec la quelle on va essayer les index (on lui rajoute le / si necessaire)
    if (this->actual_path_ressource[this->actual_path_ressource.size() - 1] == '/')
        end_slash = 1;
    else
        fake_path.append("/");
    if (this->match_location != "None")
    {   
        size_t idx = 0;
        std::cout << "size du vector index: " << this->serv.getLocation(this->match_location)->getIndex().size() << "\n";
        std::vector<std::string>::iterator its = this->serv.getLocation(this->match_location)->getIndex().end();
        std::cout << *its;
        for (std::vector<std::string>::iterator it = this->serv.getLocation(this->match_location)->getIndex().begin(); it != this->serv.getLocation(this->match_location)->getIndex().end(); ++idx)
        {
            if (idx != 1)
                it++;
            std::cout << "in 1\n";
            std::cout << *it << std::endl;
            std::string tmp = fake_path + *it;
            std::cout << "in 2\n";
            if (access(tmp.c_str(), F_OK) != -1)
            {
                std::cout << "in 3\n";
                this->actual_path_ressource = tmp;
                return ;
            }
        }
        std::cout << "apres la boucle\n";
    }
    if (end_slash && this->autoindex)
    {
        //c'est ici que tu pourras gere l'auto index, dans l'ideal tu me renvois le contenu de la page html sous une string
        //this->answer_body = fonction_garfi_auto_index(this->serv);
    }
    else
        this->autoindex = false;// si on le fait pas je le desactive ca m'est utile plus tard
}

void Parse_http::read_file()
{
    if (access(this->actual_path_ressource.c_str(), F_OK) == -1)
    {
        this->code = 404;//not found
        return ;
    }
    if (access(this->actual_path_ressource.c_str(), F_OK | R_OK) == -1)
    {
        this->code = 403;//forbidden
        return ;
    }
    int fd = open(this->actual_path_ressource.c_str(), O_RDONLY);
    if (fd == -1)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier : " << strerror(errno) << std::endl;
        return ;
    }
    const size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
        this->answer_body.append(buffer);
    close(fd);
    std::cout << YELLOW << this->answer_body << WHITE << std::endl;
}