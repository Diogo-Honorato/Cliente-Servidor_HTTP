#include "../include/handleHTTP.h"

char *readHttpRequest(int client_fd)
{

    char *request;
    char buffer[MAX_BUFFER];

    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);

    if (bytes_received < 0)
    {
        perror("ERROR::recv()\n");
        return "";
    }

    request = calloc(bytes_received, sizeof(char));

    strncpy(request, buffer, bytes_received + 1);

    return request;
}

char *readFile(const char *PATH, int client_fd)
{
    char *response;
    char *buffer;
    size_t file_size;

    FILE *file = fopen(PATH, "r");

    if (file == NULL)
    {

        perror("ERROR::FAILED TO OPEN FILE\n");
        send(client_fd, ERROR_404, sizeof(ERROR_404), 0);
        return ERROR_404;
    }

    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    //talvez pode dar segmentation fault
    fgets(buffer, file_size, file);

    fclose(file);

    //As próximas linhas arriscado lotar memoria
    char *num;
    size_t tam_str = sprintf(num,"%d",file_size);
    response = calloc((strlen(HEAD_HTML) + tam_str + 3 + file_size), sizeof(char)); // 3 = '\n\n' + /0

    sprintf(response, "%s%d\n\n%s", HEAD_HTML, file_size, buffer);

    return response;
}

void response(int client_fd)
{
    char *http_response;
    char *method;
    char *url;
    const char *path = PATH_HTML;

    char *http_request = readHttpRequest(client_fd);

    if (http_request == NULL)
    {
        close(client_fd);
        return;
    }

    method = strtok(http_request," ");
    url = strtok(NULL," ");

    strcat(path,url);//Concatena o caminho para o html

    http_response = readFile(path, client_fd);

    send(client_fd, http_response, strlen(http_response)+1, 0);

    close(client_fd);

    free(http_request);
    free(http_response);
}