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

            parseUrl(&client,url);

            if (connectToServer(&client) == -1){

                printf("TIME OUT::COULD NOT CONNECT TO THE SERVER\n");
            }
            else{

                //Logica para enviar os requests e receber os responses
            }

        }
        flush();
    }

    printf("CLIENT OFFLINE\n");

    return 0;
}