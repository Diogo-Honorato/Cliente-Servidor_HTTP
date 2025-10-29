#include "../include/main.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s 'nome_do_dominio'\n", argv[0]);
        return EXIT_FAILURE;
    }

    Client client;
    char *request = NULL;


    if (parseURI(&client, argv[1]))
    {
        return EXIT_FAILURE;
    }

    if (connectServer(&client) == -1)
    {

        printf("\n[TIME OUT::COULD NOT CONNECT TO THE SERVER]\n\n");
        close(client.client_fd);
    }
    else
    {

        request = createRequest(client.route);

        if (send(client.client_fd, request, strlen(request) + 1, MSG_NOSIGNAL) != -1)
        {

            download(client.client_fd, client.route) == 0 ? printf("\n[FILE SAVED IN PATH: download]\n\n") : printf("\n[THE DOWNLOAD COULD NOT BE COMPLETED]\n\n");
        }
        else
        {

            printf("\n[ERROR SENDING REQUEST]\n\n");
        }

        free(request);
        close(client.client_fd);
    }

    return 0;

}