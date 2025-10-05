#include "../include/server.h"

void flush() {
    int ctr;
    do {
        ctr = fgetc(stdin);
    } while (ctr != EOF && ctr != '\n');
}

int startServer(Server *server){

    server->server_fd = socket(AF_INET,SOCK_STREAM,0);

    server->addr.sin_family = AF_INET;  //IPv4
    server->addr.sin_port = htons(server->PORT_SERVER);
    server->addr.sin_addr.s_addr = inet_addr(server->IP_SERVER);

    if(bind(server->server_fd,(struct sockaddr*)&server->addr,sizeof(server->addr)) < 0){

        printf("ERROR::bind()::");
        return -1;
    }

    if(listen(server->server_fd,LISTEN_QUEUE) == -1){

        printf("ERROR::listen()::");
        return -1;
    }

    server->run = 1;

    return 0;
}

void waitConnection(Server *s){

    Server *server = (Server*)s;
    
    while(server->run){
        int client_fd;

        if((client_fd = accept(server->server_fd,NULL,NULL)) < 0){

            printf("ERROR::accept()\n");
            continue;
        }

        pthread_t client;
        pthread_create(&client,NULL,(void*)response,(void*)&client_fd);
        pthread_detach(client);
        printf("Client:[%d] inciou sessão\n",client_fd);
    }
}

int cmds(char *cmd, Server *server){

	if(strcmp(cmd,END) == 0){

		server->run = 0;

		printf("THE SERVER IS NOT ACCEPTING ANY MORE CONNECTIONS\n");

        close(server->server_fd);

        return 0;
	}
    else if(strcmp(cmd,CLEAR) == 0){

        if(system("clear") != 0){
            perror("ERROR SYSTEM\n");
        }

        CONFIGS(server->IP_SERVER,server->PORT_SERVER);

        return 0;
    }

    return -1;
}