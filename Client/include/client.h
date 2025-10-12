#ifndef CLIENT_H
#define CLIENT_H

#include "libSockets.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Se modificar o MAX_URL lembre-se de alterar o limite do <scanf("% 'MAX_URL-1' s",url)>
da main() para MAX_URL - 1*/
#define MAX_URL 91
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
int connectToServer(Client *c);
void parseUrl(Client *c,char *url);

#endif
