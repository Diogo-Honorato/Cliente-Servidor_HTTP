#include "../include/handleHTTP.h"

const char *ext[] = {"txt","html","css","gif","jpeg","png","jpg"};
const char *cont_types[] = {"text/plain","text/html","text/css","image/gif","image/jpeg","image/png","image/jpg"};


const char *mimeType(char *url){

    char *extension = NULL;
    char *temp = strtok(url,".");

    while(temp != NULL){ 
    
        extension = temp;
        temp = strtok(NULL,".");
    }

    int tam_ext = sizeof(ext)/sizeof(ext[0]);
    
    //Não e escalável mas quebra um galho por enquanto
    for(int i = 0; i < tam_ext; i++){

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

    snprintf(request, bytes_received+1, "%s",buffer);

    return request;
}

char *buildResponseHttp(unsigned short status, const char * MSG_STATUS, const char* ctn_type, size_t ctn_length,const char *body){
    
    size_t len_resp = snprintf(NULL,0,"HTTP/1.1 %hu %s\nContent-Type: %s\nContent-Length: %lu\n\n%s",status,MSG_STATUS,ctn_type,ctn_length,body) + 1; //+1 para o '\0'

    char *response = calloc(len_resp, sizeof(char));

    snprintf(response,len_resp,"HTTP/1.1 %hu %s\nContent-Type: %s\nContent-Length: %lu\n\n%s",status,MSG_STATUS,ctn_type,ctn_length,body);

    return response;
}

char *readFiles(char *url)
{
    char *buffer;
    size_t size_file;

    FILE *file = fopen(url, "rb");

    if (file == NULL)
    {   
        perror("ERROR::readFiles()");
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_file = ftell(file);
    fseek(file, 0L, SEEK_SET);

    buffer = calloc(size_file+1,sizeof(char));

    if(fread(buffer,1,size_file,file) != size_file){

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

    return buffer;
}

void response(int *client_id)
{   
    int client_fd = (int)*client_id;

    char *http_response;
    char *body;
    char *url;

    //Aguarda e processa o request do client
    char *http_request = readHttpRequest(client_fd);

    if (http_request == NULL)
    {
        close(client_fd);
        return;
    }

    //gerando tokens para extrair a url
    strtok(http_request," ");//Metodo(GET,POST,etc...)
    url = strtok(NULL," ") + 1;//evita a leitura da '/' movendo o ponteiro para o proximo endereço


    //Procura e retorna o arquivo se nao encontrar retorna 'not found'
    if((body = readFiles(url)) != NULL){

        http_response = buildResponseHttp(STATUS_OK,"OK",mimeType(url),strlen(body),body);
    }
    else{

        http_response = buildResponseHttp(STATUS_NOT_FOUND,"Not Found","text/html",strlen("<h1>404 Not Found</h1>"),"<h1>404 Not Found</h1>");        
    }

    send(client_fd, http_response, strlen(http_response)+1, 0);

    close(client_fd);
    free(http_request);
    free(http_response);
    free(body);
}