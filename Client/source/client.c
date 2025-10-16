#include "../include/client.h"

regex_t rgxs[4];
const char* pattern[] = {"^(https?:\\/\\/)","^(([0-9]{1,3}\\.){3}[0-9]{1,3})","^:([0-9]{1,5})","^(\\/.*[^\\/])$"};
const char* nameRgx[] = {"PROTOCOL","IP","PORT","PATH"};

void flush() {
    int ctr;
    do {
        ctr = fgetc(stdin);
    } while (ctr != EOF && ctr != '\n');
}

int cmds(char *cmd, Client *c){

	if(strcmp(cmd,END) == 0){

		c->run = 0;

        close(c->client_fd);

        return 0;
	}
    else if(strcmp(cmd,CLEAR) == 0){

        if(system("clear") != 0){
            perror("\n[ERROR SYSTEM]\n\n");
        }

        return 0;
    }

    return -1;
}

void startClient(Client *c){

    c->client_fd = socket(AF_INET,SOCK_STREAM,0);

    c->addr.sin_family = AF_INET; //IPv4

    c->run = true;
}

void freeRgx(int total){

    for (int i = 0; i < total; i++) {
        regfree(&rgxs[i]);
    }

}

int parseUrl(Client *c, char *uri){

    int reti;
    regmatch_t pmatch[2];
    int nmatchs = 2;
    char *it = uri;
    int tam = sizeof(rgxs) / sizeof(rgxs[0]);

    //validando uri para a tokenização
    for(int i = 0; i < tam; i++)
    {
        if((regcomp(&rgxs[i], pattern[i], REG_EXTENDED)) != 0) {
            printf("\n[ERROR COMPILING REGEX FOR: %s]\n\n", nameRgx[i]);
            freeRgx((i+1)-1);
            return EXIT_FAILURE;
        }
    }

    for(int i = 0; i < tam; i++){

        reti = regexec(&rgxs[i],it,nmatchs,pmatch,0);

        if(reti == 0){

            it += pmatch[0].rm_eo;

        }
        else if(reti == REG_NOMATCH){

            printf("\n[INCORRECTLY TYPED: %s]\n\n",nameRgx[i]);
            freeRgx(sizeof(rgxs) / sizeof(rgxs[0]));
            return EXIT_FAILURE;
        }
        else{

            char msgbuf[100];
            regerror(reti, &rgxs[i], msgbuf, sizeof(msgbuf));
            printf("\n[CRITICAL ERROR IN REGEXEC: %s]\n\n", msgbuf);
            freeRgx(sizeof(rgxs) / sizeof(rgxs[0]));
            return EXIT_FAILURE;
        }

    }

    
    //http://000.000.000.000:00000/home/user/documentos/

    strtok(uri,":"); //'http'
    c->IP_SERVER = strtok(NULL,":") + 2;// '//000.000.000.000' + 2 = '000.000.000.000'
    c->PORT_SERVER = (uint16_t)strtoul(strtok(NULL,"/"),NULL,10); // '00000'
    c->route = (strtok(NULL,"") - 1);
    *c->route = '/';  // '/home/user/documentos/'
    
    freeRgx(sizeof(rgxs) / sizeof(rgxs[0]));
    
    return EXIT_SUCCESS;
}

int connectToServer(Client *c){

    c->addr.sin_addr.s_addr = inet_addr(c->IP_SERVER);
    c->addr.sin_port = htons(c->PORT_SERVER);

    int status = -1;

    for(int i = 0; i < 6 && status == -1; i++){

        status = connect(c->client_fd,(struct sockaddr*)&c->addr,sizeof(c->addr));
        sleep(1);
    }
    
    if(status != 0){
        return -1;
    }

    return 0;
}

/*
char* createRequest(char* uri){

}*/