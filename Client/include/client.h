#ifndef CLIENT_H
#define CLIENT_H

#include "libSockets.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

/*Se modificar o MAX_URL lembre-se de alterar o limite do <scanf("% 'MAX_URL-1' s",url)>
da main() para MAX_URL - 1*/
#define MAX_URL 91
#define SIZE_BUFFER 2500001 //20Mbps + '\0'
#define END "/end"
#define CLEAR "/cls"

typedef struct
{
    bool run;

    int client_fd;
    struct sockaddr_in addr;

    char *IP_SERVER;
    uint16_t PORT_SERVER;

    char* route;
    
}Client;

void flush();
int cmds(char *cmd, Client *c);
void startClient(Client *c);
int connectServer(Client *c);
int parseUrl(Client *c,char *uri);
char* createRequest(char* uri);
int download(int client_fd,char *path);

#endif
