#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

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

    pthread_t thread_id;
    while(1){
        int client = accept(server_fd,NULL,NULL);
        if (client == -1)
            continue;
        printf("New client connected: %d\n", client);
        pthread_create(&thread_id,NULL,thread_proc,&client);
        pthread_detach(thread_id);
    }


    
    return 0;
}

void *thread_proc(void *para){

    int client = *(int *)para;
    char buf[256];
    int login = 0;

    int ret = recv(client,buf,sizeof(buf),0);
        if(ret <= 0){
            close(i);
            return NULL;
        }
        else{
            buf[ret] = 0;
            printf("Received from %d: %s\n",client,buf);
            
            if(login == 0){
                char cmd[32], id[32], tmp[32];
                ret = sscanf(buf,"%s%s%s",cmd,id,tmp);
                if(ret == 2){
                    if(strcmp(cmd,"client_id:") == 0){
                        char *msg = "Dung cu phap. Hay nhap tin nhan\n";
                        send(client,msg, strlen(msg),0);
                    }else{
                        char *msg = "Sai cu phap. Hay nhap lai.\n";
                        send(client,msg,strlen(msg),0);
                    }
                }
                else{
                    char *msg = "Sai tham so. Hay nhap lai.\n";
                    send(client,msg,strlen(msg),0);
                }
            }else{
                char sendbuff[256];
                // printf("%s\n",user_ids[i]);
                strcpy(sendbuff,user_ids[j]);
                strcat(sendbuff,": ");
                strcat(sendbuff,buf);

                for(int k =0; k < num_users; k++)
                    if(users[k] != client)
                        send(users[k],sendbuff,sizeof(sendbuff),0);
            }
        }
}