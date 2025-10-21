#include "../include/main.h"

int main()
{
    Client client;
    char uri[MAX_URL];
    char *request = NULL;

    startClient(&client);

    while (client.run)
    {   
        printf(">> ");

        if(scanf("%90s",uri) > 0){

            if(uri[0] == '/'){

                if(cmds(uri,&client) == -1){
            
                    printf("\n[COMMAND NOT FOUND]\n\n");
                }

                flush();
                continue;
            }

            if(parseUrl(&client,uri)){
                flush();
                continue;
            }

            if (connectServer(&client) == -1){

                printf("\n[TIME OUT::COULD NOT CONNECT TO THE SERVER]\n\n");
                close(client.client_fd);
            }
            else{

                request = createRequest(client.route);
                
                if(send(client.client_fd,request,strlen(request)+1,MSG_NOSIGNAL) != -1){

                    download(client.client_fd,client.route) == 0 ? printf("\n[FILE SAVED IN PATH: download]\n\n") : printf("\n[ERROR DOWNLOADING]\n\n");
                }
                else{

                    printf("\n[ERROR SENDING REQUEST]\n\n");
                }

                free(request);
                close(client.client_fd);
            }

        }
        flush();
    }

    printf("\n[CLOSED BROWSER]\n\n");

    return 0;
}