#include "../include/main.h"

int main()
{
    Client client;
    char url[MAX_URL];
    char *IP_SERVER = NULL;
    uint16_t PORT_SERVER; 


    startClient(&client);

    while (client.run)
    {   
        printf(">> ");

        if(scanf("%90s",url) > 0){

            //adiconar comandos no cliente?

            //char *parseUrl(char** IP_SERVER,uint16_t *PORT_SERVER) fazer alguma funçao de parsing para pegar o IP:PORT e o caminho

            if (connectToServer(&client,IP_SERVER,&PORT_SERVER) == -1){

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