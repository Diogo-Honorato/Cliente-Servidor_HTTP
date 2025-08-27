#ifndef HANDLEHTTP_HPP
#define HANDLEHTTP_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "libSockets.hpp"

#define MAX_BUFFER 1024
#define PATH_HTML "../../../front-end/html"
#define HEAD_HTML "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
#define ERROR_404 "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>"

std::string readHttpRequest(int client_fd);
std::string readFile(const char *PATH, int client_fd);
void response(int client_fd);


#endif