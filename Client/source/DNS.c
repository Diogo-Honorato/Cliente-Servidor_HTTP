#include "../include/DNS.h"

void toDNSFormat(unsigned char* dns, const char* host) {

    //www.teste.com para o formato DNS '3www5teste3com0'
    size_t lock = 0;
    char temp_host[MAX_HOST_LEN];
    strcpy(temp_host, host);
    strcat(temp_host, ".");

    for (size_t i = 0; i < strlen(temp_host); i++) {
        if (temp_host[i] == '.') {
            *dns++ = i - lock;
            for (; lock < i; lock++) {
                *dns++ = temp_host[lock];
            }
            lock++;
        }
    }
    *dns++ = 0x00;
}

size_t buildPacketDNS(unsigned char *buf, const char *hostname) {

    DNS *dns = (DNS *)buf;
    unsigned char *qname;
    Question *qinfo;
    unsigned char host_c[MAX_HOST_LEN];

    memset(buf, 0, BUFFER_SIZE);
    dns->id = (unsigned short)htons(getpid());
    dns->qr = 0;
    dns->rd = 1;
    dns->q_count = htons(1);


    qname = (unsigned char *)&buf[sizeof(DNS)];
    strcpy((char *)host_c, hostname);
    toDNSFormat(qname, (const char*)host_c);


    size_t qname_len = strlen((const char*)qname) + 1;
    qinfo = (Question *)&buf[sizeof(DNS) + qname_len];
    qinfo->qtype = htons(1);
    qinfo->qclass = htons(1);

    return sizeof(DNS) + qname_len + sizeof(Question);
}

in_addr_t getIPDNS(const char *domain) {

    unsigned char buffer[BUFFER_SIZE];
    int socket_fd;
    size_t packet_size;
    struct sockaddr_in dest;
    socklen_t addr_len = sizeof(dest);
    
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("ERRO: Falha ao criar socket UDP");
        return -1;
    }

    packet_size = buildPacketDNS(buffer, domain);

    dest.sin_family = AF_INET;
    dest.sin_port = htons(DNS_PORT);
    dest.sin_addr.s_addr = inet_addr(DNS_SERVER);

    //Envia Consulta
    if (sendto(socket_fd, (char *)buffer, packet_size, 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        perror("ERRO: Falha ao enviar consulta DNS");
        close(socket_fd);
        return -1;
    }

    //Recebe Resposta E CAPTURA O ENDEREÇO DO REMETENTE
    if (recvfrom(socket_fd, (char *)buffer, BUFFER_SIZE, 0, (struct sockaddr *)&dest, &addr_len) < 0) {
        perror("ERRO: Falha ao receber resposta DNS");
        close(socket_fd);
        return -1;
    }
    
    close(socket_fd);

    char IP_SERVER[IP_STR_LEN];
    
    //IP
    inet_ntop(AF_INET, &(dest.sin_addr), IP_SERVER, IP_STR_LEN);  
    
    return inet_addr(IP_SERVER);
}