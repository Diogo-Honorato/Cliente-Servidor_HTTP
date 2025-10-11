#ifndef CLIENT_H
#define CLIENT_H

#include "libSockets.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*Se modificar o MAX_URL lembre-se de alterar o limite do <scanf("% 'MAX_URL-1' s",url)>
da main() para MAX_URL - 1*/
#define MAX_URL 91


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
void startClient(Client *c);
int connectToServer(Client *c);

#endif
