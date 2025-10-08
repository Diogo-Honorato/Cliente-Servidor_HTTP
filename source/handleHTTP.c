#include "../include/handleHTTP.h"

/*
TODO:

[V]mimeType.
[]ReadFiles.

*/

const char *ext[] = {"txt","html","css","gif","jpeg","png","jpg"};
const char *cont_types[] = {"text/plain","text/html","text/css","image/gif","image/jpeg","image/png","image/jpg"};

const char *mimeType(char *url){

    char *extension;
    char *temp = strtok(url,".");

    while(temp != NULL){ 
    
        extension = temp;
        temp = strtok(NULL,".");
    }


    //Não e escalável mas quebra um galho por enquanto
    int tam = sizeof(ext)/8;

    for(int i = 0; i < tam; i++){

        if(strcmp(ext[i],extension) == 0){

            return cont_types[i];
        }
    }

    return NULL;
}

char *readHttpRequest(int client_fd)
{
    char *request;
    char buffer[MAX_BUFFER];


    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);


    if (bytes_received < 0)
    {
        perror("ERROR::recv()");
        return "";
    }

    request = calloc(bytes_received+1, sizeof(char));

    snprintf(request, bytes_received, "%s",buffer);

    return request;
}

char *readFile(const char *PATH)
{
    char *response;
    char *buffer;
    char *head_html = HEAD_HTML;
    size_t content_lenght;

    FILE *file = fopen(PATH, "rb");

    if (file == NULL)
    {
        perror("ERROR::FAILED TO OPEN FILE");
        response = malloc(strlen(ERROR_404)+1);
        sprintf(response,ERROR_404);

        return response;
    }


    fseek(file, 0L, SEEK_END);
    content_lenght = ftell(file);
    fseek(file, 0L, SEEK_SET);

    buffer = calloc(content_lenght,sizeof(char));

    if(fread(buffer,1,content_lenght,file) != content_lenght){

        if (ferror(file)) {
            perror("Error reading file");
        } 
        else {
            printf("Could not read entire file.\n");
        }

        free(buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);

    char temp[7];
    size_t str_tam = snprintf(temp,7,"%ld",content_lenght);
    int size_resp = strlen(head_html) + str_tam + 2 + content_lenght + 1;// 2 => '\n\n' 1=>'\0'
    
    response = calloc(size_resp, sizeof(char));

    //Gerando o Response completo
    snprintf(response,size_resp,"%s%ld\n\n%s",head_html, content_lenght, buffer);

    free(buffer);

    return response;
}

void response(int *client_id)
{   
    int client_fd = (int)*client_id;

    const char * path_html = PATH_HTML;
    char *http_response;
    char *url;
    char *path;
    int len_path;

    //Aguarda e processa o request do client
    char *http_request = readHttpRequest(client_fd);

    if (http_request == NULL)
    {
        close(client_fd);
        return;
    }

    //gerando tokens para pegar a url
    strtok(http_request," ");//Metodo(GET,POST,etc...)
    url = strtok(NULL," ");

    //Concatena o caminho para o html
    len_path = strlen(PATH_HTML) + strlen(url) + 1;
    path = malloc(len_path);
    snprintf(path,len_path,"%s%s",path_html,url);


    //Procura e envia o .html
    http_response = readFile(path);


    send(client_fd, http_response, strlen(http_response)+1, 0);

    close(client_fd);
    free(http_request);
    free(http_response);
    free(path);
}