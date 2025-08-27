#ifndef SERVER_HPP
#define SERVER_HPP

#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include "libSockets.hpp"
#include "handleHTTP.hpp"

#define IP_SERVER "127.0.0.1" 
#define PORT_SERVER 8080
#define LISTEN_QUEUE 10

#define END "/end"
#define ONLINE "/online"


typedef struct{

    int server_fd;
    int run;
    uint online;
    sockaddr_in addr;

}Server;

//start the server structures and configure the socket
int startServer(Server *server);

//waits for user connection requests 
void waitConnection(Server *server);

//server commands
int cmds(std::string &cmd, Server *server);

#endif