#include "../include/handleHTTP.h"

const char *ext[] = {"txt","html","css","gif","jpeg","png","jpg"};
const char *cont_types[] = {"text/plain","text/html","text/css","image/gif","image/jpeg","image/png","image/jpg"};

char *listDirectory(const char *path) {

    DIR *dir = opendir(path);
    if (!dir) {
        perror("ERROR::listDirectory()");
        return strdup("<h1>403 Forbidden or Not Found</h1>");
    }

    struct dirent *entry;
    size_t size = 1024;
    char *html = calloc(size, sizeof(char));
    snprintf(html, size, "<html><body><h1>%s</h1><ul>", path);

    while ((entry = readdir(dir)) != NULL) {
        
        // Ignora "." e ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char item_path[1024];
        snprintf(item_path, sizeof(item_path), "%s/%s", path, entry->d_name);

        // Realloc se precisar de mais espaço
        size_t needed = strlen(html) + strlen(entry->d_name) + 100;
        if (needed > size) {
            size *= 2;
            html = realloc(html, size);
        }

        snprintf(html + strlen(html), size - strlen(html),"<li><a href=\"/%s/%s\">%s</a></li>", path, entry->d_name, entry->d_name);
    }

    closedir(dir);
    snprintf(html + strlen(html), size - strlen(html), "</ul></body></html>");
    return html;
}

const char *mimeType(const char *route) {

    char tmp[256];
    strncpy(tmp, route, sizeof(tmp));
    tmp[sizeof(tmp) - 1] = '\0'; // garante terminação

    char *extension = NULL;
    char *token = strtok(tmp, ".");

    while (token != NULL) {
        extension = token;
        token = strtok(NULL, ".");
    }

    if (!extension)
        return "application/octet-stream"; // tipo padrão

    int tam_ext = sizeof(ext) / sizeof(ext[0]);
    for (int i = 0; i < tam_ext; i++) {
        if (strcmp(ext[i], extension) == 0)
            return cont_types[i];
    }

    return "application/octet-stream"; // se não encontrou
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
    
    size_t len_resp = snprintf(NULL,0,"HTTP/1.1 %hu %s\r\nContent-Type: %s\r\nContent-Length: %lu\r\n\r\n%s",status,MSG_STATUS,ctn_type,ctn_length,body) + 1; //+1 para o '\0'

    char *response = calloc(len_resp, sizeof(char));

    snprintf(response,len_resp,"HTTP/1.1 %hu %s\r\nContent-Type: %s\r\nContent-Length: %lu\r\n\r\n%s",status,MSG_STATUS,ctn_type,ctn_length,body);

    return response;
}

char *readFiles(char *route)
{
    char *buffer;
    size_t size_file;
    printf("%s\n",route);
    FILE *file = fopen(route, "rb");

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

    char *http_response = NULL;
    char *body = NULL;
    char route[512];

    char *http_request = readHttpRequest(client_fd);
    if (!http_request) {
        close(client_fd);
        return;
    }

    // Extrai a rota
    strtok(http_request, " "); // Método
    char *url = strtok(NULL, " "); 
    if (!url) url = "/";

    snprintf(route, sizeof(route), "./root%s",url);

    struct stat st;
    if (stat(route, &st) == 0 && S_ISDIR(st.st_mode)) {
        char index_path[600];
        if(strcmp(url,"/") == 0){
            snprintf(index_path, sizeof(index_path), "%sindex.html", route);
        }else{
            snprintf(index_path, sizeof(index_path), "%s/index.html", route);
        }

        if (access(index_path, F_OK) == 0) {
            body = readFiles(index_path);
            http_response = buildResponseHttp(STATUS_OK, "OK", "text/html", strlen(body), body);
        } else {
            body = listDirectory(route);
            http_response = buildResponseHttp(STATUS_OK, "OK", "text/html", strlen(body), body);
        }
    }
    else if (stat(route, &st) == 0 && S_ISREG(st.st_mode)) {
        
        body = readFiles(route);
        http_response = buildResponseHttp(STATUS_OK, "OK", mimeType(route), strlen(body), body);
    }
    else {
        
        const char *not_found = "<h1>404 Not Found</h1>";
        http_response = buildResponseHttp(STATUS_NOT_FOUND, "Not Found", "text/html", strlen(not_found), not_found);
    }

    
    send(client_fd, http_response, strlen(http_response), 0);

    close(client_fd);
    free(http_request);
    free(http_response);
    free(body);
}