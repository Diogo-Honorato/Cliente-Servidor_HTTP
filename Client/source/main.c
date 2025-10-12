#include "../include/main.h"

int main()
{
    Client client;
    char url[MAX_URL];
 

    startClient(&client);

    while (client.run)
    {   
        printf(">> ");

        if(scanf("%90s",url) > 0){

            if(url[0] == '/'){

                if(cmds(url,&client) == -1){
            
                    printf("\n[COMMAND NOT FOUND]\n\n");
                }

                flush();
                continue;
            }

            parseUrl(&client,url);

            if (connectToServer(&client) == -1){

                printf("\n[TIME OUT::COULD NOT CONNECT TO THE SERVER]\n\n");
            }
            else{

                //Logica para enviar os requests e receber os responses
            }

        }
        flush();
    }

    printf("\n[CLOSED CLIENT]\n\n");

    return 0;
}