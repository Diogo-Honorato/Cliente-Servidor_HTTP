#include "../include/client.h"

void flush() {
    int ctr;
    do {
        ctr = fgetc(stdin);
    } while (ctr != EOF && ctr != '\n');
}

int cmds(char *cmd, Client *c){

	if(strcmp(cmd,END) == 0){

		c->run = 0;

        close(c->client_fd);

        return 0;
	}
    else if(strcmp(cmd,CLEAR) == 0){

        if(system("clear") != 0){
            perror("\n[ERROR SYSTEM]\n\n");
        }

        return 0;
    }

    return -1;
}

void startClient(Client *c){

    c->client_fd = socket(AF_INET,SOCK_STREAM,0);

    c->addr.sin_family = AF_INET; //IPv4

    c->run = true;
}

void parseUrl(Client *c, char *url){

    //http://000.000.000.000:00000/home/user/documentos/

    strtok(url,":"); //'http'
    c->IP_SERVER = strtok(NULL,":") + 2;// '//000.000.000.000' + 2 = '000.000.000.000'
    c->PORT_SERVER = (uint16_t)strtoul(strtok(NULL,"/"),NULL,10); // '00000'
    c->route = (strtok(NULL,"") - 1);
    *c->route = '/';  // '/home/user/documentos/'
    
}

int connectToServer(Client *c){

    c->addr.sin_addr.s_addr = inet_addr(c->IP_SERVER);
    c->addr.sin_port = htons(c->PORT_SERVER);

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