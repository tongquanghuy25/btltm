#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

int main(int argc, char*argv[]){
// Tao socket client
    int clientfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// Tao cau truc sock_addr server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9001);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

// Connect den server
    int ret = connect(clientfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    
    if(ret < 0){
        perror("connect() failed");
        return 1;
    }

// Nhan chuoi hello tu server
    char buf[256];
    int r = recv(clientfd,buf,sizeof(buf),0);
    if(r <=0) {
        perror("recv() failed\n");
        return 0;
    }

    printf("Chuoi nhan duoc : %s \n",buf);

    while (1)
    {
        printf("Enter String : \n");

        fgets(buf,sizeof(buf),stdin);
        fflush(stdin);

        if(strncmp(buf,"exit",4) == 0)
            break;
        int sen = send(clientfd,buf,strlen(buf),0);
        if(sen <= 0) break;
        printf("%d \n",sen);

    }
    

    return 0;

}