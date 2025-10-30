#include "../include/main.h"

int main(int argc, char *argv[]){

    Server server;
    char *cmd = malloc(5);

    if (argc == 3) {
        server.IP_SERVER = argv[1];
        server.PORT_SERVER = atoi(argv[2]);
    }
    else if (argc == 1) {
        server.IP_SERVER = "127.0.0.1";
        server.PORT_SERVER = 2569;
    }
    else {
        printf("USAGE: %s <IP> <PORT>\n",argv[0]);
        exit(EXIT_FAILURE);
    }



    if(startServer(&server) == -1){

        perror("startServer()");

        close(server.server_fd);

        exit(EXIT_FAILURE);
    }

    //creating thread to accept new users
    pthread_t connection_th;
    
    pthread_create(&connection_th,NULL,(void*)waitConnection,(void*)&server);
    pthread_detach(connection_th);

    if(system("clear") != 0){
        perror("ERROR SYSTEM\n");
    }

    CONFIGS(server.IP_SERVER,server.PORT_SERVER);

    //server commands
    while(server.run){

        printf(">> ");

		if(scanf("%4s", cmd) > 0){

            if(cmds(cmd,&server) == -1){

                printf("[COMMAND NOT FOUND]\n\n");
            }
        }
        flush();
	}

    printf("SERVER OFFLINE\n");
    free(cmd);

    return 0;
}