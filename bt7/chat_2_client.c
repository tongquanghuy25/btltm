#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/select.h>

void *thread_proc(void *para);

int main(int argc, char* argv[]){

    int server_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(server_fd < 0){
        perror("Socket() failed");
        return 1;

    }
// Thiet lap dia chi id cho socket 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9001);
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



    pthread_t thread_id;
    int dem=0;
    int dbclient[2] ;

    while(1){
        int client = accept(server_fd,NULL,NULL);
        if (client == -1) continue;
        printf("New client connected: %d\n", client);
        if(dem == 0){
            char *msg = "wait next client !\n";
            send(client,msg, strlen(msg),0);
            dbclient[0] = client;
            dem = 1;
        }else{
            dbclient[1] = client;
            pthread_create(&thread_id,NULL,thread_proc,&dbclient);
            pthread_detach(thread_id);
            dem = 0;         
        }
    }


    
    return 0;
}

void *thread_proc(void *para){
    int* client = (int *)para;
    printf("%d %d\n",client[0],client[1]);
    int client1 = client[0];
    int client2 = client[1];
    char buf[256];
    char buff[256];
    sprintf(buf,"connect to client: %d\n",client2);
    printf("%s\n",buf);
    send(client1,buf,strlen(buf),0);
    sprintf(buf, "connect to client: %d\n",client1);
    send(client2,buf,strlen(buf),0);

    fd_set fd_read, fd_test;
    FD_ZERO(&fd_read);
    FD_SET(client1,&fd_read);
    FD_SET(client2,&fd_read);
    int max = (client1 > client2) ? client1 : client2;
    while(1){
        fd_test = fd_read;

        select(max + 1, &fd_test, NULL, NULL, NULL);
        if(FD_ISSET(client1,&fd_test)){
            int ret = recv(client1,buf,sizeof(buf),0);
            if(ret <= 0){
                break;
            }
            buf[ret] = 0;
            printf("clien %d: %s",client1, buf);
            sprintf(buff,"%d : ",client1);
            strcat(buff,buf);
            send(client2,buff,ret + 6,0);
        }
        if(FD_ISSET(client2,&fd_test)){
            int ret = recv(client2,buf,sizeof(buf),0);
            if(ret <= 0){
                break;
            }
            buf[ret] = 0;
            printf("clien %d: %s",client2, buf);
            sprintf(buff,"%d : ",client2);
            strcat(buff,buf);
            send(client1,buff,ret + 6,0);
        }
    }
    printf("close %d %d\n", client1, client2);
    close(client1);
    close(client2);


    // exit(NULL);
}