#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

int clients[20];
int numclient=0;
int nummenber = 0;
char names[20][64];

// typedef struct client
// {
//     char name[64];
// };



int delete(int client){
    char buff[256];
    sprintf(buff,"");
    for(int i = 0; i<numclient; i++){
        send(clients[i],)
    }
}

int checkName(char *name){
    int l = strlen(name);
    for(int i = 0; i<nummenber; i++){
        if(strncmp(name,names[i],l) == 0){
            return 200;
        }
    }
    for(int i=0; i<l; i++){
        if(!isalnum(name[i])){
            return 201;
        }
    }

    strcpy(names[nummenber], name);
    names[nummenber][l] = '\0';
    return 100;
}

int join(int client){

}

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

    fd_set fd_read, fd_test;
    FD_ZERO(&fd_read);
    int clients[20];
    int numclient=0;
    int max = server_fd;
    char buf[256];
    while (1)
    {
        fd_test = fd_read;

        int ret = select(max+1, &fd_test, NULL, NULL, NULL);
        if (ret < 0)
        {
            perror("select() failed");
            return 1;
        }

        for (int i = server_fd; i < max+1; i++)
        {
            if(FD_ISSET(i, &fd_test)){
                if (i == server_fd)
                {
                    int client = accept(server_fd, NULL, NULL);
                    if (client > max) max = client;
                    FD_SET(client, &fd_read);
                    printf("New client connected: %d\n", client);
                    numclient ++;
                    sprintf(buf, "xin moi nhap lenh 'join + {userName}' de join vao phong chat\n");
                    send(client,buf,sizeof(buf),0);
                }
            }else {
                int ret = recv(i, buf, sizeof(buf), 0);
                    if (ret <= 0)
                    {
                        FD_CLR(i, &fd_read);
                        close(i);
                    }
                buf[ret] = '\0';
                char name[64];
                if(strncmp(buf,"join"),4){
                    strcpy(name,buf+5);
                }
                
                int rs = checkName(name)
                if(rs==200){
                    char sbuf[64];
                    send()

                }
            }
        }
        

    }
    close(client_fd);
    close(server_fd);
    
    return 0;
}