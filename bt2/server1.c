#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    int server_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(server_fd < 0){
        perror("Socket() failed");
        return 1;

    }
// Thiet lap dia chi id cho socket 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.s_addr = INADDR_ANY;

// Gan dia chi cho socket
    if(bind(server_fd,(struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind() failed");
        return 1;
    }

// Lang nghe ket noi
    if(listen(server_fd,5) < 0 ){
        perror("listen() failed");
        return 1;
    }

    printf("Server listening on port 9000\n");

// Ket noi
    int client_fd = accept(server_fd, NULL, NULL);
    if(client_fd < 0){
        perror("accept() failed");
        return 1;
    }
    printf("Client connected ! \n");

// Gui xau den client
    char *mes = "Hello client ! \n";
    int sen = send(client_fd,mes,strlen(mes),0);
    if(sen <= 0){
        perror("send failed");
        return 0;
    }



// Nhan tin nhan tu client

    char buf[1024];
    while (1)
    {
        int rec = recv(client_fd,buf,sizeof(buf),0);
        if(rec <=0 ) {
            printf("ket noi bi dong \n");
            break;
        }
        if(rec < 1024)
            buf[rec] = 0;
        printf("%s", buf);
    }
    close(client_fd);
    close(server_fd);
    
    return 0;
}