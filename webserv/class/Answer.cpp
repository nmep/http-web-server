#include "Answer.hpp"

Answer::Answer(int server_idx)
{
    this->server_idx = server_idx;
    this->status = 0;
    this->socket_fd = -2;
    this->code = 200;
    this->cgi = false;
    this->nb_readfile = 0;
    this->sum_len = 0;
    this->step = 0;

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
    this->mime_map[".js"] = "application/javascript";
    this->mime_map[".py"] = "text/x-python";
    this->mime_map[".php"] = "application/x-httpd-php";

    this->code_map[100] = "Continue";
    this->code_map[101] = "Switching Protocol";
    this->code_map[200] = "OK";// use
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
    this->code_map[400] = "Bad Request";// use
    this->code_map[401] = "Unauthorized";
    this->code_map[403] = "Forbidden";// use
    this->code_map[404] = "Not Found";//use
    this->code_map[405] = "Method Not Allowed";
    this->code_map[406] = "Not Acceptable";
    this->code_map[407] = "Proxy Authentication Required";
    this->code_map[408] = "Request Timeout";
    this->code_map[409] = "Conflict";
    this->code_map[410] = "Gone";
    this->code_map[411] = "Length Required";
    this->code_map[412] = "Precondition Failed";
    this->code_map[413] = "Payload Too Large";//use
    this->code_map[414] = "URI Too Long";
    this->code_map[415] = "Unsupported Media Type";// use
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
    this->code_map[500] = "Internal Server Error";//use
    this->code_map[501] = "Not Implemented";
    this->code_map[502] = "Bad Gateway";
    this->code_map[503] = "Service Unavailable";
    this->code_map[504] = "Gateway Timeout";
    this->code_map[505] = "HTTP Version Not Supported";//use
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
// une fois qu'on a la loc on peut savoir si la methode est valide ou pas
// la difference entre l'apelle dans get et dans post c'est que dans post on veux pas que ca bloque si la ressource ne correspond pas a une location (genre /upload)
void Answer::find_ressource_path(Configuration const &conf)
{
    int depth = -1;
    std::map<std::string, Location*> map = conf.getServer(this->server_idx).getLocationMap();
    std::map<std::string, Location*>::iterator it = map.begin();
    for (; it != map.end(); it++) {
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
        // std::cout << YELLOW << it->first << " et " << it ->second << " " << this->ressource << " " << this->match_location << " " << this->ressource_path << WHITE << std::endl;
    }
    if (depth == -1 && this->methode == "GET")
    {
        this->ressource_path = this->ressource;
        this->match_location = "None";
        this->code = 404; 
        return ;
    }
    else if (depth == -1 && this->methode == "POST")
    {
        this->ressource_path = this->ressource;
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

void Answer::find_good_index_or_autoindex(Configuration const &conf)
{
    std::string fake_path = this->ressource_path;
    if (this->ressource_path[this->ressource_path.size() - 1] != '/')
        fake_path.append("/");
    if (this->match_location != "None")
    {   
        this->autoindex = conf.getServer(this->server_idx).getLocation(this->match_location)->getAutoIndex();
        std::vector<std::string> vec = conf.getServer(this->server_idx).getLocation(this->match_location)->getIndex();
        for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
        {
            std::string tmp = fake_path + *it;
            if (access(tmp.c_str(), F_OK) != -1)
            {
                this->ressource_path = tmp;
                return ;
            }
        }
    }
    if (this->autoindex == true)
    {
        //c'est ici que tu pourras gere l'auto index, dans l'ideal tu me renvois le contenu de la page html sous une string
        //this->answer_body = fonction_garfi_auto_index();
        this->answer_body = "<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>autoindex</title>\n</head>\n<body>\n<h1>example</h1>\n<h1>de</h1>\n<h1>dossier</h1>\n</body>\n</html>\n";
        this->status = 3;
    }
    else
        this->code = 404;
}

bool Answer::isScript()
{
    std::vector<std::string> tab;
    tab.push_back(".py");
    tab.push_back(".php");
    size_t dot = this->ressource_path.find_last_of('.');
    if (dot != std::string::npos && dot < this->ressource_path.size() && this->mime_map.find(this->ressource_path.substr(dot)) != this->mime_map.end())
    {
        if (std::find(tab.begin(), tab.end(), this->ressource_path.substr(dot)) != tab.end())
            return true;
    }
    return false;
}

bool Answer::isBinary()
{
    std::vector<std::string> tab;
    tab.push_back(".html");
    tab.push_back(".htm");
    tab.push_back(".txt");
    tab.push_back(".css");
    size_t dot = this->ressource_path.find_last_of('.');
    if (dot != std::string::npos && dot < this->ressource_path.size() && this->mime_map.find(this->ressource_path.substr(dot)) != this->mime_map.end())
    {
        if (std::find(tab.begin(), tab.end(), this->ressource_path.substr(dot)) == tab.end())
            return true;
    }
    return false;
}
// quand on an fini de lire la request, on la parse et on indique quelle sera la prochaine etape
void Answer::DoneWithRequest(Configuration const &conf)
{
    if (this->methode == "GET")
        this->GET(conf);
    else if (this->methode == "POST")
        this->POST(conf);
    else if (this->methode == "DELETE")
        this->DELETE();

    return ;
}

void Answer::HandleError(Configuration const &conf)
{
    std::stringstream tmp;
    tmp << this->code;
    std::map<std::string, std::string> map = conf.getServer(this->server_idx).getErrorPageMap();
    if (map.find(tmp.str()) != map.end())
    {
        this->status = 1;
        this->fd_read = open(map[tmp.str()].c_str(), O_RDONLY);
        if (this->fd_read == -1)
            this->code = 500;// normalement les fichiers sont tester a la config et ne peuvent pas fail ici, c'est juste une precaution, a voir ce qu'on fait quand 500 est dans la liste des error page
        else
            return ;
    }
    // si on a pas la page dans la config, on genere une page d'erreur par default
    this->status = 3;
    std::stringstream tmp2;
    tmp2 << this->code;
    this->answer_body.clear();// je sais pas si je dois forcement le clear ici, est ce qu'on peut avoir une erreur apres avoir commencer a remplir le body ?? todo
    this->answer_body = "<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Error " + tmp2.str() + "</title>\n<style>\nh1 { text-align: center; margin-top: 10%; }\nh2 { text-align: center; }\n</style>\n</head>\n<body>\n<h1>Error " + tmp2.str() + " - " + this->GetCodeSentence(this->code) + "</h1>\n<h2>Default page</h2>\n</body>\n</html>\n";
}

void Answer::parse_state_line(std::string state_line)
{
    this->piece_of_request = this->piece_of_request.substr(this->piece_of_request.find_first_of("\r\n") + 2);
    size_t start = 0;
    size_t end = state_line.find_first_of(' ');
    if (end == std::string::npos)
    {
        this->code = 400;// requete mal faite
        return ;
    }
    this->methode = state_line.substr(start, end);
    std::cout << this->methode << std::endl;
    state_line = state_line.substr(end + 1);
    start = 0;
    end = state_line.find_first_of(' ');
    if (end == std::string::npos)
    {
        this->code = 400;// requete mal faite
        return ;
    }
    this->ressource = state_line.substr(start, end);
    // on cherche si on a des parametres cgi
    size_t lim = this->ressource.find_last_of('?');
    if (lim != std::string::npos && lim != 0 && lim != this->ressource.size())
    {
        this->cgi_env_var = this->ressource.substr(lim + 1);
        this->ressource = this->ressource.substr(0, lim);
    }
    std::cout << this->ressource << std::endl;
    state_line = state_line.substr(end + 1);
    if (state_line != "HTTP/1.1")
    {
        this->code = 505;// http pas supporte
        return ;
    }
}

void Answer::parse_header(std::string header)
{
    size_t end = 0;
    size_t start = 0;
    std::vector<std::string> header_line;
	while (end != std::string::npos)// on prend le header ligne par ligne et on trime
	{
		end = header.find_first_of("\r\n", start);
		if (end > start) {
			std::string line = header.substr(start, end - start);
			size_t last = line.find_last_not_of("\r\n");
			if (last != std::string::npos)
				line = line.substr(0, last + 1);
            header_line.push_back(line);
		}
		start = end + 2;
	}
    for (size_t i = 0; i < header_line.size(); i++)
    {
        size_t colon = header_line[i].find(":");
        if (colon == std::string::npos)
        {
            this->code = 400;
            return ;
        }
        this->header_map[header_line[i].substr(0, colon)] = header_line[i].substr(colon + 2);
    }
    for (std::map<std::string, std::string>::iterator it = this->header_map.begin(); it != this->header_map.end(); it++)
    {
        std::cout << it->first << " et " << it->second << std::endl;
    }
    std::cout << "sorti\n";
}

//quand on a pas encore eu la ligne d'etat en entier
void Answer::first_step(size_t bytesRead)
{
    if (this->piece_of_request.find("\r\n\r\n") != std::string::npos)
    {
        this->step = 2;
        this->parse_state_line(this->piece_of_request.substr(0, this->piece_of_request.find_first_of("\r\n")));
        if (this->code >= 400)
            return ;
        this->piece_of_request = this->piece_of_request.substr(this->piece_of_request.find_first_of("\r\n") + 2);
        this->parse_header(this->piece_of_request.substr(0, this->piece_of_request.find("\r\n\r\n")));
        if (this->code >= 400)
            return ;
        this->piece_of_request = this->piece_of_request.substr(this->piece_of_request.find("\r\n\r\n") + 4);
        if (bytesRead < READ_SIZE)
            this->request_body = this->piece_of_request;
        else
            this->remaining_part = this->piece_of_request;
    }
    else if (this->piece_of_request.find("\r\n") != std::string::npos)
    {
        this->step = 1;
        this->parse_state_line(this->piece_of_request.substr(0, this->piece_of_request.find_first_of("\r\n")));
        if (this->code >= 400)
            return ;
        this->piece_of_request = this->piece_of_request.substr(this->piece_of_request.find_first_of("\r\n") + 2);
        if (this->piece_of_request.find("\r\n") != std::string::npos)
        {
            this->parse_header(this->piece_of_request.substr(0, this->piece_of_request.find_last_not_of("\r\n") + 1));
            if (this->code >= 400)
                return ;
            this->piece_of_request = this->piece_of_request.substr(this->piece_of_request.find("\r\n\r\n") + 4);

        }
        this->remaining_part = this->piece_of_request;
    }
    else
    {
        this->remaining_part = this->piece_of_request;
    }
}

void Answer::second_step(size_t bytesRead)
{
    if (this->piece_of_request.find("\r\n\r\n") != std::string::npos)
    {
        this->step = 2;
        this->parse_header(this->piece_of_request.substr(0, this->piece_of_request.find("\r\n\r\n")));
        if (this->code >= 400)
            return ;
        this->piece_of_request = this->piece_of_request.substr(this->piece_of_request.find("\r\n\r\n") + 4);
        if (bytesRead < READ_SIZE)
            this->request_body = this->piece_of_request;
        else
            this->remaining_part = this->piece_of_request;
    }
    else if (this->piece_of_request.find("\r\n") != std::string::npos)
    {
        this->parse_header(this->piece_of_request.substr(0, this->piece_of_request.find_last_not_of("\r\n") + 1));
        if (this->code >= 400)
            return ;
        this->piece_of_request = this->piece_of_request.substr(this->piece_of_request.find("\r\n\r\n") + 4);
        this->remaining_part = this->piece_of_request;
    }
    else
    {
        this->remaining_part = this->piece_of_request;
    }
}

void Answer::third_step(size_t bytesRead)
{
    if (this->piece_of_request.size() >= 10000)//temporaire apres je pourrais prendre ta variable max size body todo
        this->code = 413;
    if (bytesRead < READ_SIZE)
        this->request_body = this->piece_of_request;
    else
        this->remaining_part = this->piece_of_request;
}

// on lit la requete par tranche de READ_SIZE et quand on a fini on la parse avec DoneWithRequest
void Answer::ReadRequest(Configuration const &conf, int socket_fd)
{
    std::cout << RED << "Debut de ReadRequest" << WHITE << std::endl;
    this->piece_of_request.clear();
    this->piece_of_request.append(this->remaining_part);
    if (this->socket_fd == -2)
        this->socket_fd = socket_fd;
    char buffer[READ_SIZE];
    ssize_t bytesRead = recv(this->socket_fd, buffer, READ_SIZE, 0);
    if (bytesRead == -1)
        this->code = 500;//error server
    buffer[bytesRead] = '\0';
    this->request.append(buffer);
    this->piece_of_request.append(buffer);
    sum_len += bytesRead;

    if (this->step == 0)
        this->first_step(bytesRead);
    else if (this->step == 1)
        this->second_step(bytesRead);
    else if (this->step == 2)
        this->third_step(bytesRead);
    
    if (bytesRead < READ_SIZE || this->code >= 400)// si on a fini de lire la requete
    {
        if (this->code < 400)// si la lecture a marcher
        {
            if (this->methode == "GET")
                this->GET(conf);
            else if (this->methode == "POST")
                this->POST(conf);
            else if (this->methode == "DELETE")
                this->DELETE();
        }
        if (this->code >= 400)
            this->HandleError(conf);
    }
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
    char buffer[READ_SIZE];
    int bytesRead;

    if (this->cgi == true && this->nb_readfile == 0)
    {
        this->nb_readfile = 1;
        int status;
        std::cerr << "hereeee\n";
        int result = waitpid(this->cgi_pid, &status, 0);
        std::cerr << "outtt\n";
        std::cout << RED << "result " << result << " fin" << WHITE << std::endl;
        if (!WIFEXITED(status))
            this->code = 500;// a voir comment on determine que ca a fail todo
    }
    bytesRead = read(this->fd_read, buffer, READ_SIZE);
    if (bytesRead == -1) {
		this->code = 500;//gerer comment on traiter la page d'erreur depuis ici ?
        close(this->fd_read);
        return ;
    }
    buffer[bytesRead] = '\0';
    this->answer_body.append(buffer);// dans le cas d'un cgi c'est pas vraiment le body, il y a aussi des headers
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

char** Answer::ft_build_env(Configuration const &conf) {
    std::vector<std::string> env_vars;

    env_vars.push_back("REQUEST_METHOD="+this->methode);
    env_vars.push_back("CONTENT_TYPE=application/x-www-form-urlencoded");
    if (!cgi_env_var.empty() && this->methode == "GET") {
        env_vars.push_back("QUERY_STRING=" + cgi_env_var);
    }
    env_vars.push_back("SERVER_NAME=" + conf.getServer(server_idx).GetServerName());
    if(this->header_map.find("Connection") != this->header_map.end() && this->header_map["Connection"] == "keep-alive")
        env_vars.push_back("CONNECTION=keep-alive");
    char date[1000];
    time_t now = time(0) + 7200;//7200 ca vaut 2h en secondes
    struct tm tm = *gmtime(&now);
    strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
    env_vars.push_back("DATE=" + std::string(date));


    char** envp = new char*[env_vars.size() + 1];
    for (size_t i = 0; i < env_vars.size(); ++i) {
        envp[i] = strdup(env_vars[i].c_str());
    }
    envp[env_vars.size()] = NULL;

    return envp;
}

void Answer::WriteFile(Configuration const &conf)
{
    std::cout << RED << "Debut de WriteFile" << WHITE << std::endl;

    if (this->cgi == true)
    {
        this->status = 1;
        int pipe_in[2];
        int pipe_out[2];
        if (pipe(pipe_in) == -1)
        {
            this->code = 500;
            this->status = 3;
            return ;
        }
        if (pipe(pipe_out) == -1)
        {
            this->code = 500;
            this->status = 3;
            close(pipe_in[0]);
            close(pipe_in[1]);
            return ;
        }
        this->fd_read = pipe_out[0];
        this->fd_write = pipe_in[1];
        
        this->cgi_pid = fork();
        if (this->cgi_pid == -1)
        {
            this->code = 500;
            return ;
        }
        if (this->cgi_pid == 0)
        {
            dup2(pipe_in[0], STDIN_FILENO); // Redirige l'entrée
            dup2(pipe_out[1], STDOUT_FILENO); // Redirige la sortie
            close(pipe_in[1]);
            close(pipe_out[0]);
            std::string extension = this->ressource_path.substr(this->ressource_path.find_last_of('.'));
            char *exec_path;
            if (extension == ".py")
                exec_path = (char*)"/usr/bin/python3";
            else
                exec_path = (char*)"/usr/bin/php";
            char *argv[] = { exec_path, (char*)this->ressource_path.c_str(), NULL };
            char **envp = this->ft_build_env(conf);
            size_t x = 0;
            while (envp[x] != NULL)
            {
                std::cerr << envp[x] << std::endl;
                x++;
            }
            if (execve(exec_path, argv, envp) == -1)
            {
                size_t size = 0;
                while (envp[size] != NULL)
                    size++;
                for (size_t i = 0; i <= size; i++) {
                    free(envp[i]);
                }
                delete[] envp;
                std::cerr << RED << "fail execve" << WHITE << std::endl;
                // est ce qu'on close les sockets ?? todo
                this->code = 500;// comment faire pour faire savoir qu'on a fail, doit on le faire savoir ?? to do
                // kill(this->cgi_pid, SIGKILL);
                exit(EXIT_FAILURE);// on a pas le droit a exit, a voir ce qu'on fait todo
            }
        }
        else
        {
            close(pipe_in[0]); // Ferme le côté lecture
            write(this->fd_write, this->cgi_env_var.c_str(), this->cgi_env_var.size());// a voir si on ecrit en plusieurs fois, je pense que oui mais je veux deja voir si ca marche
            close(this->fd_write);
            std::cout << MAGENTA << "ici " << this->cgi_env_var << std::endl << WHITE;
            
            close(pipe_out[1]); // Ferme le côté écriture
        }
        
    }
    else
    {
        // ecrire un fichier classique
    }


    std::cout << RED << "Fin de WriteFile" << WHITE << std::endl;
}

void Answer::SendAnswer(Configuration const &conf)
{
    std::cout << RED << "Debut de SendAnswer" << WHITE << std::endl;
    std::stringstream tmp;
    tmp << this->code;

    if (this->cgi == true)
    {
        this->answer.append(this->answer_body);
    }
    else
    {
        this->answer.append("HTTP/1.1 " + tmp.str() + " " + this->GetCodeSentence(this->code) + "\r\n");
        this->contentType();
        this->connection();
        this->server(conf);
        this->location();
        this->date();
        this->taille();
        this->answer.append("\n");
        // on remplit le body
        if (this->answer_body.size() != 0)
            this->answer.append(this->answer_body);
    }
    send(this->socket_fd, this->answer.c_str(), strlen(this->answer.c_str()), 0);
    // if(this->header_map.find("Connection") == this->header_map.end() || this->header_map["Connection"] != "keep-alive")
    //     close(this->socket_fd);// la close ailleur ou pas ?? normalement meme si on collapse avant on send quand meme donc non
    close(this->socket_fd);// la close ailleur ou pas ?? normalement meme si on collapse avant on send quand meme donc non
    std::cout << YELLOW << this->answer << WHITE << std::endl;
    std::cout << RED << "Fin de SendAnswer" << WHITE << std::endl;
    this->Reset();
}

void Answer::contentType()
{
    if(this->code >= 400 || this->cgi == true)// est ce qu'on affiche quand meme que c'est du html en cas de page d'erreur?? todo
        return;
    this->answer.append("Content-Type: ");
    size_t dot = this->ressource_path.find_last_of('.');
    if (dot != std::string::npos && dot < this->ressource_path.size() && this->mime_map.find(this->ressource_path.substr(dot)) != this->mime_map.end())
        this->answer.append(this->GetMime(this->ressource_path.substr(dot)));
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
    size_t len;
    if (this->cgi == true)// quand c'est le retour d'un cgi, answer body a aussi les headers que le cgi a renvoyer
    {
        if (this->answer_body.find("\n\n") != std::string::npos)
        len = this->answer_body.substr(this->answer_body.find("\n\n")).size();

    }
    else
        len = this->answer_body.size();
    std::stringstream tmp;
    tmp << len;
    this->answer.append("Content-Length: " + tmp.str() + "\r\n");
}

void Answer::Reset()
{
    this->status = 0;
    this->code = 200;
    this->socket_fd = -2;
    this->cgi = false;
    this->nb_readfile = 0;
    this->answer.clear();
    this->answer_body.clear();

    this->request.clear();
    this->methode.clear();
    this->ressource_path.clear();
    this->cgi_env_var.clear();
    this->header_map.clear();
    this->request_body.clear();
    this->sum_len = 0;
    this->remaining_part.clear();
    this->step = 0;
                 
    // peut etre qu'on reset aussi l'auto index
    this->match_location.clear();
}

void Answer::GET(Configuration const &conf)
{
    this->find_ressource_path(conf);
    if (this->code >= 400)
        return ;
    if (this->is_that_a_directory() == 1)
    {
        this->find_good_index_or_autoindex(conf);
        if (this->status == 3)
            return ;
    }
    // on a tout de pres pour savoir si l'extension
    size_t dot = this->ressource_path.find_last_of('.');
    if (dot == std::string::npos || (dot < this->ressource_path.size() && this->mime_map.find(this->ressource_path.substr(dot)) == this->mime_map.end()))
        this->code = 415;

    this->status = 1;
    if (this->code >= 400)
    {
        // si le chemin est un dossier sans autoindex et sans index qui marche
    }
    else
    {
        if (access(this->ressource_path.c_str(), F_OK) == -1)
        {
            this->code = 404;//not found
            return ;
        }
        else if (access(this->ressource_path.c_str(), F_OK | R_OK) == -1)
        {
            std::cout << "1 " << this->ressource << " et " << this->ressource_path << std::endl;
            this->code = 403;//forbidden
            return ;
        }
        else if (this->isScript() == true && access(this->ressource_path.c_str(), F_OK | R_OK | X_OK) == -1)
        {
            std::cout << "2 " << this->ressource << " et " << this->ressource_path << " et " << this->cgi_env_var << std::endl;
            this->code = 403;//forbidden
            return ;
        }
        if (this->isScript() == true)
        {  
            this->cgi = true;
            this->status = 2;
        }
        else if (this->isBinary() == true)// si on a besoin de le lire en binaire
        {
            this->fd_read = open(this->ressource_path.c_str(), O_RDONLY);
            if (this->fd_read == -1)
            {
                std::cerr << "Erreur lors de l'ouverture du fichier" << std::endl;
                this->code = 500;// a voir quelle code on met quand le fichier ne s'ouvre pas  todo
                return ;
            }
        }
        else
        {
            this->fd_read = open(this->ressource_path.c_str(), O_RDONLY);
            if (this->fd_read == -1)
            {
                std::cerr << "Erreur lors de l'ouverture du fichier" << std::endl;
                this->code = 500;// a voir quelle code on met quand le fichier ne s'ouvre pas  todo
                return ;
            }
        }
    }
}

void Answer::cgi_from_post()
{
    size_t start = 0;
	size_t end = 0;
    std::string line;
	while (end != std::string::npos)
	{
		end = this->request_body.find("\r\n", start);
		if (end > start) {
			std::string str = this->request_body.substr(start, end - start);
			size_t first = str.find_first_not_of(" \t\n\r\f\v");
			size_t last = str.find_last_not_of(" \t\n\r\f\v");
			if (first != std::string::npos && str.substr(first, last - first + 1).find('=') != std::string::npos)
            {
				this->cgi_env_var = str.substr(first, last - first + 1);
                break ;
            }
		}
		start = end + 1;
	}
    this->status = 2;
}

void Answer::POST(Configuration const &conf)
{   
    this->find_ressource_path(conf);
    if (this->code >= 400)
        return ;
    if (this->isScript() == true)
    {
        if (access(this->ressource_path.c_str(), F_OK) == -1)
        {
            this->code = 404;//not found
            return ;
        }
        else if (access(this->ressource_path.c_str(), F_OK | X_OK) == -1)
        {
            std::cout << "1 " << this->ressource << " et " << this->ressource_path << std::endl;
            this->code = 403;//forbidden
            return ;
        }
        this->cgi = true;
        this->find_ressource_path(conf);
        if (this->code >= 400)
            return ;
        this->cgi_from_post();
        if (this->code >= 400)
        {
            this->status = 3;
            return ;
        }
        this->status = 2;
        
        return ;
    }

    this->status = 1;
    if (this->code >= 400)
    {
        return ;
    }


    // met toi ici GARFI, tu peux faire l'upload file ici
    std::cout << GREEN << this->ressource << std::endl;
    std::cout << this->request_body << WHITE << std::endl;
    this->code = 201;//quand post marche
    this->status = 2; // si il faut ecrire quelque chose (la fonction writefile est vide tu peux faire la suite la bas)
}

void Answer::DELETE()
{

}