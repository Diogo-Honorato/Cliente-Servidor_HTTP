#ifndef SERVER_HPP
#define SERVER_HPP

#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "libSockets.h"
#include "handleHTTP.h"

#define LISTEN_QUEUE 10

#define END "/end"
#define ONLINE "/online"

typedef struct{

    int server_fd;
    int run;
    sockaddr_in addr;

    const char *IP_SERVER;
    int PORT_SERVER;

}Server;

//start the server structures and configure the socket
int startServer(Server *server);

//waits for user connection requests 
void waitConnection(Server *server);

//server commands
int cmds(char *cmd, Server *server);

#endif