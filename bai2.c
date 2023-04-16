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
    char *output_file = argv[3];

    int server_id = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(server_id < 0){
        perror("Socket() failed");
        return 1;

    }

    struct sockaddr_in addr;
    addr


    return 0;
}