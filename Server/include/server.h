#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>

#include "handleHTTP.h"

#define LISTEN_QUEUE 10

#define END "/end"
#define CLEAR "/cls"

#define CONFIGS(IP,PORT) printf("SERVER STARTED\nIP:PORT = %s:%d\n\n", IP,PORT)

typedef struct{

    int server_fd;
    int run;
    struct sockaddr_in addr;

    const char *IP_SERVER;
    int PORT_SERVER;

}Server;

//clean buffer stdin
void flush();

//start the server structures and configure the socket
int startServer(Server *server);

//waits for user connection requests 
void waitConnection(Server *server);

//server commands
int cmds(char *cmd, Server *server);

#endif