#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../networking.h"

#define SERVER_PORT 9090
#define BUFFER_SIZE 256

int main() 
{
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
            continue;
        }
        write(STDOUT_FILENO, buffer, nread);
        printf("\n");

        char temp[] = "Answer message\0";
        write(fd, temp, strlen(temp));
        sleep(15);
        close(fd);
    }
    
    close(server);
    return 0;
}
