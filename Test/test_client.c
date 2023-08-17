#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../networking.h"

#define SERVER_PORT 8080
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) 
{
    int soc = Socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    Inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    socklen_t addrlen = sizeof(server);
    Connect(soc, &server, addrlen);


    char message[BUFFER_SIZE];
    if(argc == 2){
        strcpy(message, argv[1]);
        printf("%s\n", message);
    }
    else{
        strcpy(message, "Test message\0");
    }
    
    write(soc, message, strlen(message));

    ssize_t nread;
    char buffer[BUFFER_SIZE];
    nread = read(soc, buffer, BUFFER_SIZE);
    if(nread == -1){
            perror("Read fail");
    }
    else{
        write(STDOUT_FILENO, buffer, nread);
        printf("\n");
    }
    close(soc);
    return 0;
}
