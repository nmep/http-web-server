#include "parse_http.hpp"
#include "Socket.hpp"
// #include "tools.cpp"
std::vector<std::string>	split_by_Pierre(std::string line);

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
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	write(socket.clientFd, hello, strlen(hello));
    close(socket.clientFd);
	return 1;
}

int Parse_http::GetData()
{
    line = split_by_Pierre(std::string(this->request));
    return 1;
}

void Parse_http::PrintData()
{
    std::cout << std::endl << RED << this->request << WHITE << std::endl;
    std::cout << YELLOW;
    for (size_t i = 0; i < this->line.size(); ++i) {
        std::cout << this->line[i] << std::endl << std::endl;
    }
    std::cout << WHITE;
}