#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

int main(int argc, char *argv[]){
    if(argc != 4){
        printf("Nhap sai tham so dong lenh");
        return 0;
    }

    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int reciever = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

// Khai bao dia ch ben nhan
    struct sockaddr_in receiv_addr;
    receiv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    receiv_addr.sin_family = AF_INET;
    receiv_addr.sin_port = htons(atoi(argv[2]));

// Khai bao dia chi ben may
    struct sockaddr_in send_addr;
    send_addr.sin_family = AF_INET;
    send_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    send_addr.sin_port = htons(atoi(argv[3]));

    bind(reciever,(struct sockaddr *)&send_addr, sizeof(send_addr));


    fd_set fdread, fdtest;

    FD_ZERO(&fdread);
    FD_SET(STDIN_FILENO,&fdread);
    FD_SET(reciever, &fdread);

    char buf[256];
    while(1){
        fdtest = fdread;

        int ret = select(reciever +1, &fdtest, NULL, NULL, NULL);

        if (ret < 0)
        {
            perror("select() failed");
            return 1;
        }

        if (ret == 0)
        {
            printf("Timed out!!!\n");
            continue;
        }

        if(FD_ISSET(STDIN_FILENO,&fdtest)){
            fgets(buf, sizeof(buf),stdin);
            sendto(sender, buf, sizeof(buf), 0, (struct sockaddr *)&receiv_addr, sizeof(receiv_addr));
        }

        if(FD_ISSET(reciever,&fdtest)){
            int ret = recvfrom(reciever, buf, sizeof(buf), 0, NULL, NULL);
            if(ret <= 0 ){
                printf("recvfrom() failed \n");
                continue;
            }else{
                buf[ret] = '\0';
                printf("receiv: %s",buf);
            }
        }
    }
    return 0;
}