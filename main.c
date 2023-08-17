#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "config.h"
#include "networking.h"

#define SERVER_PORT 8080
#define SUPERIOR_SERVER_PORT 9090
#define BUFFER_SIZE 256

int main() 
{
    //Load config
    struct DNSConfig config;
    config.blacklist_length = 0;
    config.server_address = NULL;
    config.server_reply = NULL;

    if (loadConfig(&config) != 0){
        return 1;
    }
    
    int superior_server = Socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in superior_server_address = {0};
    superior_server_address.sin_family = AF_INET;
    superior_server_address.sin_port = htons(SUPERIOR_SERVER_PORT);
    Inet_pton(AF_INET, config.server_address, &superior_server_address.sin_addr);

    Connect(superior_server, &superior_server_address, sizeof superior_server_address);
    printf("Connected\n");
    int server = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    Bind(server, (struct sockaddr*) &server_address, sizeof server_address);

    Listen(server, 1);
    printf("Listen\n");

    while(1)
    {
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);
        int fd = Accept(server, (struct sockaddr*) &client_address, &addrlen);

        ssize_t nread;
        char buffer[BUFFER_SIZE];
        nread = read(fd, buffer, BUFFER_SIZE);
        if(nread == -1){
            perror("Read fail");
        }
        if(nread == 0){
            write(fd, "Empty message\0", 14);
            continue;;
        }
        write(STDOUT_FILENO, buffer, nread);
        printf("\n");
        
        if(isBlacklisted(&config, &buffer) == 1){
            write(fd, config.server_reply, strlen(config.server_reply));
            sleep(15);
            close(fd);
            continue;
        }

        write(superior_server, buffer, nread);
        nread = read(superior_server, buffer, BUFFER_SIZE);
        write(STDOUT_FILENO, buffer, nread);
        printf("\n");
        write(fd, buffer, nread);
        sleep(15);
        close(fd);
    }
    
    clearConfig(&config);
    return 0;
}