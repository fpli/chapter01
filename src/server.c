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

int main(int argc, char const *argv[])
{
    int sockfd, servlen, n, ret;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Creating socket");
        exit(1);
    }

    struct sockaddr_in serv_addr, cli_addr;   
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5001);
    servlen = sizeof(serv_addr);

    ret = bind(sockfd, (struct sockaddr *)&serv_addr, servlen);
    if (ret < 0) {
        perror("bind");
        exit(1);
    }

    listen(sockfd, 5);

    int newsockfd;
    socklen_t clilen;
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Accepting");
        exit(1);
    }
    printf("A connection has been established.\n");

    for (int i = 0; i < 100; i++)
    {
        /* code */
        char str[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        char arr[3 + strlen(str)];
        sprintf(arr, "%d%s", i, str);
        write(newsockfd, arr, strlen(arr));
        sleep(1);
    }
    

    char buf[BUFSIZ];
    while ((n = read(newsockfd, buf, sizeof(buf))) > 0)
    {
        printf("n: %d \n", n);
        write(STDOUT_FILENO, buf, n);
        for (int i = 0; i < n; i++)
        {
            buf[i] = toupper(buf[i]);
        }   
        char *msg = "I got your message.\n";
        write(newsockfd, msg, strlen(msg));
        write(newsockfd, buf, n);
    }
    
    close(newsockfd);
    
    close(sockfd);

    sleep(60);
    return 0;
}
