#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>

void *thread_proc(void *);

int main() {
    int listener = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);
    addr.sin_family = AF_INET;
    if(bind(listener,(struct sockaddr*) &addr,sizeof(addr))){
        printf("Error: %d - %s\n",errno,strerror(errno));
    }
    if(listen(listener,5)==-1){
        printf("Error: %d - %s\n",errno,strerror(errno));
    }

    pthread_t thread_id;
    while(1){
        int client = accept(listener,NULL,NULL);
        if (client == -1)
            continue;
        printf("New client connected: %d\n", client);
        pthread_create(&thread_id,NULL,thread_proc,(void *)&client);
        pthread_detach(thread_id);
    }

    return 0;
}

void *thread_proc(void *para){

    int client = *(int *)para;
    char buf[256];

    while (1)
    {
        int data = 0;
        strcpy(buf,"Welcome client! Please enter your username password\n");
        send(client,buf,strlen(buf),0);
        //Valid username and password
        while(1){
            data = recv(client,buf,sizeof(buf),0);
            int login = 0;
            if(data==0){
                close(client);
                break;
            }
            buf[data-1] = 0;
            char username[10];
            char password[10];
            char somthing_else[10];
            int number_valid = sscanf(buf,"%s %s %s",username,password,somthing_else);
            if(number_valid!=2){
                strcpy(buf,"Please enter correct format username password\n");
                send(client,buf,strlen(buf),0);
                continue;
            }
            else{
                FILE * file = fopen("user_login.txt","r");
                while(!feof(file)){
                    char name[10];
                    char pass[10];
                    fscanf(file,"%s %s",name,pass);
                    if(strcmp(name,username)==0&&strcmp(pass,password)==0){
                        strcpy(buf,"Welcome client\n\0");
                        send(client,buf,strlen(buf),0);
                        login = 1;
                        break;
                    }
                }
            if(login==1){
                break;
            }
            else{
                printf("Username or password not correct\n");
            }
            }
        }
        if(data==0){
            break;
        }
        //Respone request from client
        while(1){
            data = recv(client,buf,sizeof(buf),0);
            if(data<=0){
                printf("Connection lost\n");
                break;
            }
            else{
                buf[data-1] = 0;
                char output[20] = " > out.txt";
                strcat(buf,output);
                system(buf);
                FILE * f = fopen("out.txt","r");
                while(fgets(buf,sizeof(buf),f)){
                    send(client,buf,strlen(buf),0);
                }
                fclose(f);
            }
        }
        close(client);
    }
    
    return NULL;
}