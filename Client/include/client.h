#ifndef CLIENT_H
#define CLIENT_H

#include "libSockets.h"
#include "DNS.h"
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
    int client_fd;
    struct sockaddr_in addr;

    in_addr_t IP_SERVER;
    uint16_t PORT_SERVER;

    char* DNS;

    char* route;
    
}Client;

int parseDNS(Client *c,char *uri);
int parseIpPort(Client *c,char *uri);
int parseURI(Client *c,char *uri);
int connectServer(Client *c);
char* createRequest(char* uri);
uint16_t logStatusCodeHttp(char *header);
int download(int client_fd,char *path);

#endif
