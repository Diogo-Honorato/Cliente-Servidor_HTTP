#ifndef DNS_H
#define DNS_H

#include "libSockets.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define DNS_SERVER "8.8.8.8" //IP do Google Public DNS
#define DNS_PORT 53

#define BUFFER_SIZE 65536
#define MAX_HOST_LEN 100
#define IP_STR_LEN 16

//Cabeçalho DNS (12 bytes)
typedef struct{
    unsigned short id;
    
    // 16 bits de flags
    unsigned char rd :1;        // Recursion Desired
    unsigned char tc :1;        // Truncated Message
    unsigned char aa :1;        // Authoritative Answer
    unsigned char opcode :4;    // Query Type
    unsigned char qr :1;        // Query/Response Flag

    unsigned char rcode :4;     // Response Code
    unsigned char z :3;         // Reserved
    unsigned char ra :1;        // Recursion Available

    unsigned short q_count;     //total de perguntas
    unsigned short ans_count;   //total de respostas
    unsigned short auth_count;  //total de servidores de autoridade
    unsigned short add_count;   //total de registros adicionais

}DNS;

typedef struct  {
    unsigned short qtype;   // Tipo de consulta
    unsigned short qclass;  // Classe

}Question;


#pragma pack(push, 1) //compactação network format

typedef struct {
    unsigned short type;
    unsigned short _class;
    unsigned int ttl;
    unsigned short data_len;

}RecordData;

#pragma pack(pop)

void toDNSFormat(unsigned char* dns, const char* host);
size_t buildPacketDNS(unsigned char *buf, const char *hostname);
in_addr_t getIPDNS(const char *domain);

#endif