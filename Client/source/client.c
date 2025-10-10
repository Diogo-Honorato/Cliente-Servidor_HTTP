#include "../include/client.h"

void flush() {
    int ctr;
    do {
        ctr = fgetc(stdin);
    } while (ctr != EOF && ctr != '\n');
}

void startClient(Client *c){

    c->client_fd = socket(AF_INET,SOCK_STREAM,0);

    c->addr.sin_family = AF_INET; //IPv4

    c->run = true;
}

int connectToServer(Client *c, char* IP_SERVER, uint16_t *PORT_SERVER){

    c->addr.sin_addr.s_addr = inet_addr(IP_SERVER);
    c->addr.sin_port = htons(*PORT_SERVER);

    int status = -1;

    for(int i = 0; i < 6 && status == -1; i++){

        status = connect(c->client_fd,(struct sockaddr*)&c->addr,sizeof(c->addr));
        sleep(1);
    }
    
    if(status != 0){
        return -1;
    }

    return 0;
}