#ifndef HANDLEHTTP_HPP
#define HANDLEHTTP_HPP

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "libSockets.h"

#define MAX_BUFFER 1024
#define PATH_HTML "html"
#define HEAD_HTML "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
#define ERROR_404 "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>"

char * readHttpRequest(int client_fd);
char * readFile(const char *PATH);
void response(int *client_fd);


#endif