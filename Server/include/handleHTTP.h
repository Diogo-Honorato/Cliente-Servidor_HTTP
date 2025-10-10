#ifndef HANDLEHTTP_H
#define HANDLEHTTP_H

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "libSockets.h"

#define MAX_BUFFER 1024

#define STATUS_OK 200
#define STATUS_NOT_FOUND 404


char *readHttpRequest(int client_fd);
char *buildResponseHttp(unsigned short status, const char * MSG_STATUS, const char* ctn_type, size_t ctn_length,const char *body);
char *readFiles(char *url);
void response(int *client_fd);

#endif