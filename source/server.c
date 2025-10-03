#include "../include/server.h"

int startServer(Server *server){

    server->server_fd = socket(AF_INET,SOCK_STREAM,0);

    server->addr.sin_family = AF_INET;  //IPv4
    server->addr.sin_port = htons(PORT_SERVER);
    server->addr.sin_addr.s_addr = inet_addr(IP_SERVER);

    if(bind(server->server_fd,(struct sockaddr*)&server->addr,sizeof(server->addr)) < 0){

        std::cerr << "ERROR::bind()::";
        return -1;
    }

    if(listen(server->server_fd,LISTEN_QUEUE) == -1){

        std::cerr << "ERROR::listen()::";
        return -1;
    }

    server->run = 1;
    server->online = 0;

    return 0;
}

void waitConnection(Server *server){

    int client_fd;

    while(server->run){
        
        if((client_fd = accept(server->server_fd,NULL,NULL)) < 0){

            std::cerr << "ERROR::accept()\n";
            continue;
        }
        
        std::thread(response,client_fd).detach();

    }
}

int cmds(char *cmd, Server *server){

	if((cmd == END)){

		server->run = 0;

		printf("THE SERVER IS NOT ACCEPTING ANY MORE CONNECTIONS\n");

        close(server->server_fd);

        return 0;
	}

    return -1;
}