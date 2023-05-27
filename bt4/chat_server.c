#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <ctype.h>


int main(int argc, char* argv[]){

    if(argc != 2) {
        printf("Nhap sai tham so dong lenh ");
        return 0;
    }

    int listener = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(listener < 0){
        perror("Socket() failed");
        return 1;

    }
// Thiet lap dia chi id cho socket 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = INADDR_ANY;

// Gan dia chi cho socket
    if(bind(listener,(struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind() failed");
        return 1;
    }

// Lang nghe ket noi
    if(listen(listener,5) < 0 ){
        perror("listen() failed");
        return 1;
    }

    printf("Server listening on port 9000\n");

    fd_set fdread, fdtest;
    
    // Xóa tất cả socket trong tập fdread
    FD_ZERO(&fdread);
    
    // Thêm socket listener vào tập fdread
    FD_SET(listener, &fdread);

    char buf[256];
    
    struct timeval tv;

    int numclient = 0, max = listener;

    while(1){
        fdtest = fdread;

        // Chờ đến khi sự kiện xảy ra
        int ret = select(max+1, &fdtest, NULL, NULL, NULL);

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
        for (int i = listener; i < max+1; i++)
        {
            if (FD_ISSET(i, &fdtest))
            {
                if (i == listener)
                {
                    int client = accept(listener, NULL, NULL);
                    if (client > max) max = client;
                    FD_SET(client, &fdread);
                    printf("New client connected: %d\n", client);
                    numclient ++;
                    sprintf(buf, "Dang co %d client ket noi\n", numclient);
                    send(client,buf,sizeof(buf),0);
                }
                else{
                    int ret = recv(i, buf, sizeof(buf), 0);
                    if (ret <= 0)
                    {
                        FD_CLR(i, &fdread);
                        close(i);
                    }
                    else
                    {
                        buf[ret] = '\0';
                        char *str = buf;

                        while (*(str) == ' ')
                        {
                            str += 1;
                        }

                        while (*(str + strlen(str) - 1) == ' ')
                        {
                            *(str + strlen(str) - 1) = '\0';
                        }
                        char *buff = (char *) malloc(256);
                        int j = 0;
                        int d = 1;
                        buff[0] = toupper(*(str));
                        while(*(str + j + 1) != '\0'){
                        if(*(str + j) == ' ' && *(str + j + 1) == ' ' ){
                            j++;
                        }else{
                            if(*(str + j) == ' ' && *(str + j + 1) != ' ' ){
                                buff[d] = toupper(*(str + j + 1));
                            }else{
                                buff[d] = tolower(*(str + j + 1));
                            }
                            j++;
                            d++;
                        } 
    }

                        printf("Received from %d: %s\n", i, buff);
                        send(i,buff,strlen(buff),0);

                    }
                }
            }
        }
    }
    return 0;
}