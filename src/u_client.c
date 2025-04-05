/* a client in the unix domain */
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stddef.h>

#define UNIX_SOCK "u_server.sock"
#define CLIE_SOCK "clie.sock"

void error(const char *);

int main(int argc, char *argv[])
{
    int sockfd, servlen, n;
    struct sockaddr_un serv_addr, cli_addr;

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        error("Creating socket");

    // begin optional
    bzero(&cli_addr, sizeof(cli_addr));
    cli_addr.sun_family = AF_UNIX;
    strcpy(cli_addr.sun_path, CLIE_SOCK);
    socklen_t len = offsetof(struct sockaddr_un, sun_path) + strlen(cli_addr.sun_path);

    bind(sockfd, (struct sockaddr *)&cli_addr, len);
    // end

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, UNIX_SOCK);
    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0)
        error("Connecting");
        
    char buffer[BUFSIZ];
    bzero(buffer, sizeof(buffer));
    
    printf("Please enter your message: ");
    fgets(buffer, sizeof(buffer), stdin);
    
    write(sockfd, buffer, strlen(buffer));
    
    bzero(buffer, sizeof(buffer));
    n = read(sockfd, buffer, sizeof(buffer));
    printf("The return message was: %s\n", buffer);

    write(STDOUT_FILENO, buffer, n);

    close(sockfd);
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}