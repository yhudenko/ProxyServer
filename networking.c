#include "networking.h"


#include <stdio.h>
#include <stdlib.h>

int Socket(int domain, int type, int protocol)
{
    int res = socket(domain, type, protocol);
    if(res == -1){
        perror("Socket fail\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
    int res = bind(sockfd, addr, addrlen);
    if(res == -1){
        perror("Socket fail\n");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog)
{
    int res = listen(sockfd, backlog);
    if(res == -1){
        perror("Listen fail\n");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen)
{
    int res = accept(sockfd, addr, addrlen);
    if(res == -1){
        perror("Accept fail\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Connect(int sockfd, struct sockaddr* addr, socklen_t addrlen)
{
    int res = connect(sockfd, addr, addrlen);
    if(res == -1){
        perror("Connect fail\n");
        exit(EXIT_FAILURE);
    }
}

void Inet_pton(int af, const char* src, void* dst)
{
    int res = inet_pton(af, src, dst);
    if(res == -1 || res == 0){
        perror("Inet_pton fail\n");
        exit(EXIT_FAILURE);
    }
}