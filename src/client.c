/**
 * @file client.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <syslog.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
    int sockfd, servlen, n, ret;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Creating socket");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    serv_addr.sin_port = htons(5001);
    servlen = sizeof(serv_addr);

    ret = connect(sockfd, (struct sockaddr *)&serv_addr, servlen);  
    if (ret < 0) {
        perror("Connecting");
        exit(1);
    }

    printf("Please enter your message: ");
    char buffer[BUFSIZ];
    bzero(buffer, sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin);
    write(sockfd, buffer, strlen(buffer));

    n = read(sockfd, buffer, sizeof(buffer));

    printf("The return message was:\n");
    write(STDOUT_FILENO, buffer, n);
    
    close(sockfd);

    return 0;
}
