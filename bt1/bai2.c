#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 4){
        printf("Nhap sai tham so\n");
        return 1;
    }

    int port = atoi(argv[1]);
    char *hello_file = argv[2];
    char *data_file = argv[3];

    int server_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(server_fd < 0){
        perror("Socket() failed");
        return 1;

    }
// Thiet lap dia chi id cho socket 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
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

    printf("Server listening on port %d \n",port);

// Ket noi
    int client_fd = accept(server_fd, NULL, NULL);
    if(client_fd < 0){
        perror("accept() failed");
        return 1;
    }
    printf("Client connected ! \n");

// Doc xau vao tep tin
    FILE *hello = fopen(hello_file,"r");
    if(hello == NULL){
        perror("fopen() failed \n");
        return 1;
    }

    char hello_buf[256];
    fgets(hello_buf,256,hello);

// Gui xau den client
    int sen = send(client_fd,hello_buf,strlen(hello_buf),0);
    if(sen <= 0){
        perror("send failed");
        return 0;
    }



// Nhan tin nhan tu client

FILE *data = fopen(data_file,"w");
    char buf[256];
    while (1)
    {
        int rec = recv(client_fd,buf,sizeof(buf),0);
        if(rec <=0 ) {
            printf("ket noi bi dong \n");
            break;
        }
        if(rec < 256)
            buf[rec] = 0;
        printf("Da nhan %s", buf);
        fprintf(data,"%s",buf);
    }
    close(client_fd);
    close(server_fd);
    
    return 0;
}