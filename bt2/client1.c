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
    server_addr.sin_port = htons(9000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

// Connect den server
    int ret = connect(clientfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    
    if(ret < 0){
        perror("connect() failed");
        return 1;
    }
// Nhan chuoi hello tu server
    char buf[1024];
    int r = recv(clientfd,buf,sizeof(buf),0);
    if(r <=0) {
        perror("recv() failed\n");
        return 0;
    }

    printf("Chuoi nhan duoc : %s \n",buf);
    printf("Nhap ten may tinh : \n");
    scanf("%s",buf);
    char *pc = &buf[strlen(buf)];
    
    int sodia;
    printf("Nhap so o dia : \n");
    scanf("%d",&sodia);

    char tendia[1];
    char dungluong[10];
    for(int i =1; i<=sodia; i++)
    {
        printf("Nhap ten o dia : \n");
        fflush(stdin);
        scanf("%s",tendia);
        sprintf(pc,"\n%s - ",tendia);
        pc = &buf[strlen(buf)];
        printf("Nhap dung luong :\n");
        fflush(stdin);
        scanf("%s",dungluong);
        sprintf(pc,"%s",dungluong);
        pc = &buf[strlen(buf)];

    }

    int sen = send(clientfd,buf,sizeof(buf),0);
    printf("So byte da gui : %d\n",sen);
    printf("%s\n",buf);
    return 0;
    

    return 0;

}