#include "../include/main.h"

int main()
{
    Client client;
    char uri[MAX_URL];
 

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

            if (connectToServer(&client) == -1){

                printf("\n[TIME OUT::COULD NOT CONNECT TO THE SERVER]\n\n");
            }
            else{

                //char* createRequest(char* uri);
            }

        }
        flush();
    }

    printf("\n[CLOSED CLIENT]\n\n");

    return 0;
}