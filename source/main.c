#include "../include/main.h"

int main(int argc, char *argv[]){

    Server server;
    char *cmd;

    if(argc == 3){
		server.IP_SERVER = argv[1];
		server.PORT_SERVER = atoi(argv[2]);
	}
	else if(argc == 1){
        //if you do not define anything, local host will be defined
		server.IP_SERVER = "127.0.0.1";
		server.PORT_SERVER = 2569;
	}
	else{
		perror("ERROR AND EXPECTED 2 ARGS\n");
		exit(EXIT_FAILURE);
	}


    if(startServer(&server) == -1){

        perror("startServer()\n");

        close(server.server_fd);

        return -1;
    }

    printf("SERVER STARTED\nIP:PORT %s:%d\n", server.IP_SERVER,server.PORT_SERVER);

    //creating thread to accept new users
    pthread_t connection_th;
    
    pthread_create(&connection_th,NULL,waitConnection,(void*)&server);
    pthread_detach(connection_th);

    //server commands
    while(server.run){

		scanf("%s", cmd);

        if(cmds(cmd,&server) == -1){

            printf("COMMAND NOT FOUND\n");
        }
		
		fflush(stdin);
	}

    printf("SERVER OFFLINE\n");

    return 0;
}