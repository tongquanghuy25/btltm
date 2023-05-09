#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

int main(int argc, char* argv[]){

    int server_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(server_fd < 0){
        perror("Socket() failed");
        return 1;

    }
// Thiet lap dia chi id cho socket 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9004);
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

    // printf("Server listening on port 9000\n");

    fd_set fdtest, fdread;

    FD_ZERO(&fdread);

    FD_SET(server_fd,&fdread);

    char buf[256];
    int users[64];
    char *user_ids[64];
    int num_users = 0;

    while ((1))
    {
        fdtest = fdread;

        int ret = select(FD_SETSIZE,&fdtest,NULL,NULL,NULL);

        if(ret < 0){
            perror("select() failed");
            return 1;
        }

        if(ret == 0){
            printf("Time out!!\n");
            continue;
        }
        
        for(int i = server_fd; i < FD_SETSIZE; i++){
            if(FD_ISSET(i,&fdtest)){
                if(i==server_fd){
                    int client = accept(server_fd,NULL,NULL);
                    if(client < FD_SETSIZE){
                        FD_SET(client,&fdread);
                        printf("New client connected : %d\n",client);
                    }
                    else{
                        close(client);
                    }
                }
                else{
                    int ret = recv(i,buf,sizeof(buf),0);
                    if(ret <= 0){
                        FD_CLR(i,&fdread);
                        close(i);
                    }
                    else{
                        buf[ret] = 0;
                        printf("Received from %d: %s\n",i,buf);

                        int client = i;

                        int j = 0;
                        for(;j < num_users; j++)
                            if(users[j] == client)
                                break;
                        
                        if(j == num_users){
                            char cmd[32], id[32], tmp[32];
                            ret = sscanf(buf,"%s%s%s",cmd,id,tmp);
                            if(ret == 2){
                                if(strcmp(cmd,"client_id:") == 0){
                                    char *msg = "Dung cu phap. Hay nhap tin nhan\n";
                                    send(client,msg, strlen(msg),0);

                                    users[num_users] = client;
                                    user_ids[num_users] = malloc(strlen(id)+1);
                                    strcpy(user_ids[num_users], id);
                                    num_users ++;
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
            }
        }

    }

    close(server_fd);
    
    return 0;
}