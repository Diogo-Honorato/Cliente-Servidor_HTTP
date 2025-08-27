#include "../include/handleHTTP.hpp"

std::string readHttpRequest(int client_fd){

    std::string request;
    char buffer[MAX_BUFFER];

    while (true)
    {

        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);

        if (bytes_received < 0)
        {
            std::cerr << "ERROR::recv()\n";
            return "";
        }
        if (bytes_received == 0)
        {

            break;
        }

        request.append(buffer, bytes_received);

        if (request.find("\r\n\r\n") != std::string::npos)
        {
            break;
        }
    }

    return request;
}

std::string readFile(const char *PATH, int client_fd)
{
    std::string response;
    std::stringstream buffer;
    std::streamsize readBytes;
    char *error_404 = ERROR_404;

    std::ifstream file(PATH);

    if (!file.is_open()){

        std::cerr << "ERROR::FAILED TO OPEN FILE\n";
        send(client_fd, error_404, sizeof(ERROR_404), 0);
        return "";
    }

    buffer << file.rdbuf();

    readBytes = file.gcount();

    file.close();

    response = HEAD_HTML + std::to_string(readBytes) + "\n\n" + buffer.str();

    return response;
}

void response(int client_fd)
{
    std::string http_response;
    std::string method;
    std::string url;
    std::string path(PATH_HTML);

    std::string http_request(readHttpRequest(client_fd));

    if (http_request.empty())
    {
        close(client_fd);
        return;
    }

    std::istringstream iss(http_request);
    iss >> method >> url;

    path.append(url);

    http_request = readFile(path.c_str(),client_fd);

    send(client_fd, http_response.c_str(), http_response.size(), 0);

    close(client_fd);
}