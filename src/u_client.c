/* a client in the unix domain */
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void error(const char *);

int main(int argc, char *argv[])
{
    int sockfd, servlen, n;
    struct sockaddr_un serv_addr;
    char buffer[BUFSIZ];

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, argv[1]);
    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

    // printf("offsetof: %d \n", __offsetof(struct sockaddr_un, sun_path));   

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        error("Creating socket");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0)
        error("Connecting");

    printf("Please enter your message: ");
    bzero(buffer, sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin);

    write(sockfd, buffer, strlen(buffer));
    n = read(sockfd, buffer, sizeof(buffer));
    printf("The return message was:\n");
    write(STDOUT_FILENO, buffer, n);
    printf("%d",  INET_ADDRSTRLEN);
    close(sockfd);
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}