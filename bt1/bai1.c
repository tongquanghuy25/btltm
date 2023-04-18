#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char*argv[]){

    int sockfd;
    struct sockaddr_in serveraddr;
    char buf[256];

    if(argc != 3){
        printf("Sai tham so\n");
        return 1;
    }

    sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0){
        perror("Socket() failed");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET,argv[1],&serveraddr.sin_addr);


    if(connect(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0){
        perror("Connect() failed");
        return 1;
    }

    while (1)
    {
        printf("Enter string:");
        fgets(buf,sizeof(buf),stdin);

        send(sockfd,buf,strlen(buf),0);

        if(strncmp(buf,"exit",4) == 0){
            break;
        }

    }
    
    close(sockfd);
    return 0;
}
