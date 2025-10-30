#include "../include/client.h"

int parseDNS(Client *c, char *uri){

    regex_t rgxs;
    int reti = 0;
    const char *pattern = "^https?:\\/\\/(www\\.)[-a-zA-Z0-9@:%._+~#=]{1,256}\\.[a-zA-Z0-9()]{1,6}([-a-zA-Z0-9@:%._+~#?&\\/=]*)$";

    // validando uri com regex para a tokenização
    if ((regcomp(&rgxs, pattern, REG_EXTENDED)) != 0)
    {
        char msgbuf[100];
        regerror(reti, &rgxs, msgbuf, sizeof(msgbuf));
        printf("\n[ERROR COMPILING REGEX FOR: %s]\n\n", msgbuf);
        regfree(&rgxs);
        return 1;
    }

    reti = regexec(&rgxs, uri, 0, NULL, 0);

    if (reti == 0)
    {
        // tokenização
        //  http://www.site.com/home/user/documentos/imagem.png

        char *ptr_host = strstr(uri, "://");
        ptr_host += 3;
        ptr_host = strchr(ptr_host, '/');

        if(ptr_host == NULL || *(ptr_host + 1) == '\0'){

            strtok(uri, "/");                                             //'http:'
            c->DNS = strtok(NULL, "/");                                   // 'www.site.com'
            c->route = "/";

        }else{

            strtok(uri, "/");                                             //'http:'
            c->DNS = strtok(NULL, "/");                                  // 'www.site.com'
            c->route = (strtok(NULL, "") - 1);
            *c->route = '/'; // '/home/user/documentos/imagem.png'
        }

        regfree(&rgxs);


        //Conecta com Server DNS e pega o IP e PORT do Dominio correspondent

        c->IP_SERVER = getIPDNS(c->DNS);

        if(c->IP_SERVER == (in_addr_t)-1){

            return 1;
        }

        if (strstr(uri, "https:")){

            c->PORT_SERVER = 443;

        }else {
            c->PORT_SERVER = 80;
        }

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

int parseIpPort(Client *c, char *uri){

    int reti = 0;
    regex_t rgxs;
    const char *pattern = "^(https?:\\/\\/)([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})(:[0-9]{1,5})(\\/.*)$";

    // validando uri com regex para a tokenização
    if ((regcomp(&rgxs, pattern, REG_EXTENDED)) != 0)
    {
        char msgbuf[100];
        regerror(reti, &rgxs, msgbuf, sizeof(msgbuf));
        printf("\n[ERROR COMPILING REGEX FOR: %s]\n\n", msgbuf);
        regfree(&rgxs);
        return 1;
    }

    reti = regexec(&rgxs, uri, 0, NULL, 0);

    if (reti == 0)
    {
        // tokenização
        //  http://000.000.000.000:00000/home/user/documentos/imagem.png

        char *ptr_host = strstr(uri, "://");
        ptr_host += 3;
        ptr_host = strchr(ptr_host, '/');

        if(*(ptr_host + 1) == '\0'){

            strtok(uri, ":");                                                //'http'
            c->IP_SERVER = inet_addr(strtok(NULL, ":") + 2);                 // '//000.000.000.000' + 2 = '000.000.000.000'
            c->PORT_SERVER = (uint16_t)strtoul(strtok(NULL, "/"), NULL, 10); // '00000'
            c->route = "/";

        }else{

            strtok(uri, ":");                                                //'http'
            c->IP_SERVER = inet_addr(strtok(NULL, ":") + 2);                 // '//000.000.000.000' + 2 = '000.000.000.000'
            c->PORT_SERVER = (uint16_t)strtoul(strtok(NULL, "/"), NULL, 10); // '00000'
            c->route = (strtok(NULL, "") - 1);
            *c->route = '/'; // '/home/user/documentos/'
        }

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

int parseURI(Client *c, char *uri)
{   
    uint8_t option = 0;

    if(strstr(uri,"www")){
        option = 1;
    }

    switch (option)
    {
    case 0:
        return parseIpPort(c,uri);

    case 1:
        return parseDNS(c,uri);

    default:
        break;
    }

    return 1;
}

int connectServer(Client *c)
{
    c->client_fd = socket(AF_INET, SOCK_STREAM,0);

    c->addr.sin_family = AF_INET; // IPv4
    c->addr.sin_addr.s_addr = c->IP_SERVER;
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
    const char *template = "GET %s HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";

    size_t len_req = snprintf(NULL, 0, template, path);
    char *buffer = calloc(len_req + 1, sizeof(char));

    snprintf(buffer, len_req + 1, template, path);

    return buffer;
}


int download(int client_fd, char *path)
{
    char *buffer = calloc(SIZE_BUFFER, sizeof(char));
    ssize_t recv_bytes = recv(client_fd, buffer, SIZE_BUFFER, 0);

    if (recv_bytes <= 0)
    {
        printf("\n[FAILED TO RECEIVE DATA]\n\n");
        free(buffer);
        return 1;
    }

    char *ptr_ctn = strstr(buffer, "\r\n\r\n");
    if (!ptr_ctn)
    {
        printf("\n[ERROR PARSING RESPONSE BODY]\n\n");
        free(buffer);
        return 1;
    }
    ptr_ctn += 4;


    if (strstr(buffer, "404 Not Found"))
    {
        printf("\n[ERROR 404 NOT FOUND]\n\n");
        free(buffer);
        return 1;
    }


    char temp_path[strlen(path) + 1];
    strcpy(temp_path, path);

    char *file_name = NULL;
    if (strlen(path) == 1)
    {
        file_name = "index.html";
    }
    else
    {
        char *temp = strtok(temp_path, "/");
        while (temp != NULL)
        {
            file_name = temp;
            temp = strtok(NULL, "/");
        }
    }


    char path_local[512];
    snprintf(path_local, sizeof(path_local), "download/%s", file_name);

    FILE *output = fopen(path_local, "wb");
    if (output == NULL)
    {
        perror("\nERROR download():");
        free(buffer);
        return 1;
    }


    size_t header_size = ptr_ctn - buffer;
    fwrite(ptr_ctn, 1, recv_bytes - header_size, output);


    while (true)
    {
        recv_bytes = recv(client_fd, buffer, SIZE_BUFFER, 0);
        if (recv_bytes <= 0)
            break;

        fwrite(buffer, 1, recv_bytes, output);
    }

    fclose(output);
    free(buffer);

    return 0;
}