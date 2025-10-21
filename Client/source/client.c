#include "../include/client.h"

const char *pattern = "^(https?:\\/\\/)([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})(:[0-9]{1,5})(\\/.*[^\\/])$";
const char *nameRgx[] = {"PROTOCOL", "IP", "PORT", "PATH"};

void flush()
{
    int ctr;
    do
    {
        ctr = fgetc(stdin);
    } while (ctr != EOF && ctr != '\n');
}

int cmds(char *cmd, Client *c)
{
    if (strcmp(cmd, END) == 0)
    {

        c->run = 0;

        close(c->client_fd);

        return 0;
    }
    else if (strcmp(cmd, CLEAR) == 0)
    {

        if (system("clear") != 0)
        {
            perror("\n[ERROR SYSTEM]\n\n");
        }

        return 0;
    }

    return -1;
}

void startClient(Client *c)
{
    c->addr.sin_family = AF_INET; // IPv4

    c->run = true;
}

int parseUrl(Client *c, char *uri)
{
    int reti = 0;
    regex_t rgxs;
    regmatch_t pmatch[5];
    int nmatchs = 5;

    // validando uri com regex para a tokenização
    if ((regcomp(&rgxs, pattern, REG_EXTENDED)) != 0)
    {
        char msgbuf[100];
        regerror(reti, &rgxs, msgbuf, sizeof(msgbuf));
        printf("\n[ERROR COMPILING REGEX FOR: %s]\n\n", msgbuf);
        regfree(&rgxs);
        return 1;
    }

    reti = regexec(&rgxs, uri, nmatchs, pmatch, 0);

    if (reti == 0)
    {
        // tokenização
        //  http://000.000.000.000:00000/home/user/documentos/

        strtok(uri, ":");                                                //'http'
        c->IP_SERVER = strtok(NULL, ":") + 2;                            // '//000.000.000.000' + 2 = '000.000.000.000'
        c->PORT_SERVER = (uint16_t)strtoul(strtok(NULL, "/"), NULL, 10); // '00000'
        c->route = (strtok(NULL, "") - 1);
        *c->route = '/'; // '/home/user/documentos/'

        regfree(&rgxs);
    }
    else if (reti == REG_NOMATCH)
    {

        printf("\n[ERROR URI ENTERED INCORRECTLY]\n\n");
        regfree(&rgxs);
        return 1;
    }
    else
    {
        char msgbuf[100];
        regerror(reti, &rgxs, msgbuf, sizeof(msgbuf));
        printf("\n[CRITICAL ERROR IN REGEXEC: %s]\n\n", msgbuf);
        regfree(&rgxs);
        return 1;
    }

    return 0;
}

int connectServer(Client *c)
{
    c->client_fd = socket(AF_INET, SOCK_STREAM, 0);

    c->addr.sin_addr.s_addr = inet_addr(c->IP_SERVER);
    c->addr.sin_port = htons(c->PORT_SERVER);

    int status = -1;

    for (int i = 0; i < 6 && status == -1; i++)
    {

        status = connect(c->client_fd, (struct sockaddr *)&c->addr, sizeof(c->addr));
        sleep(1);
    }

    if (status != 0)
    {
        return -1;
    }

    return 0;
}

char *createRequest(char *path)
{

    size_t len_req = snprintf(NULL, 0, "GET %s HTTP/1.1", path);

    char *buffer = calloc(len_req + 1, sizeof(char));

    snprintf(buffer, len_req + 1, "GET %s HTTP/1.1", path);

    return buffer;
}

int download(int client_fd, char *path)
{
    FILE *output = fopen(path,"wb");

    if (output == NULL)
    {   
        perror("\nERROR download():");
        return 1;
    }

    char *buffer = calloc(SIZE_BUFFER,sizeof(char));

    ssize_t recv_bytes = recv(client_fd, buffer, SIZE_BUFFER, 0);

    if (recv_bytes < 0)
    {
        printf("\n[ERROR FAILED TO RECEIVE DATA]\n\n");
        fclose(output);
        free(buffer);
        return 1;
    }
    
    char *ptr_ctn = strstr(buffer,"\n\n") + 2;

    strtok(buffer," ");
    if(strcmp(strtok(NULL," "),"404") == 0){

        printf("\n[ERROR 404 NOT FOUND]\n\n");
        fclose(output);
        free(buffer);
        return 1;
    }

    fwrite(ptr_ctn,sizeof(char),recv_bytes-((ptr_ctn - buffer)+2),output);

    //verifica se tem mais bytes a serem recebidos
    while (true)
    {
        ssize_t recv_bytes = recv(client_fd, buffer, SIZE_BUFFER, 0);

        if (recv_bytes == 0)
        {
            break;
        }

        fwrite(buffer,sizeof(char),recv_bytes,output);
    }

    fclose(output);
    free(buffer);

    return 0;
}