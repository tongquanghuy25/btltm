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

const char TIME_FORMAT[4][20] = {"dd/mm/yyyy","dd/mm/yy","mm/dd/yyyy","mm/dd/yy"};


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
    while (1)
    {
        int client = accept(listener,NULL,NULL);
        if(client == -1)
            continue;
        printf("New client connected: %d",client);
        pthread_create(&thread_id,NULL,thread_proc,(void *)&client);
        pthread_detach(thread_id);
    }
    return 0;
}

void* thread_proc(void* para){
    int client = *(int *)para;
    char buf[256];
    while(1){
        int data = recv(client,buf,sizeof(buf),0);
        buf[data-1] = 0; 
        int i;
        for(i = 0; i < 4; i++){
            if(!strcmp(buf,TIME_FORMAT[i])){
                break;
            }
        }
        if(i==4){
            strcpy(buf,"Please enter right format!\n\0");
            send(client,buf,strlen(buf),0);
        }
        else{
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                memset(buf,'\0',sizeof(buf));
                switch (i)
                {
                case 0:
                    strftime(buf, sizeof(buf), "%d/%m/%Y", t);
                    break;
                case 1:
                    strftime(buf, sizeof(buf), "%d/%m/%Y", t);
                    buf[6] = buf[8];
                    buf[7] = buf[9];
                    buf[8] = 0;
                    buf[9] = 0;
                    break;                        
                case 2:
                    strftime(buf, sizeof(buf), "%m/%d/%Y", t);
                    break;                        
                case 3:
                    strftime(buf, sizeof(buf), "%d/%m/%Y", t);
                    buf[6] = buf[8];
                    buf[7] = buf[9];
                    buf[8] = 0;
                    buf[9] = 0;
                    break;
                default:
                    break;
                }
                buf[strlen(buf)] = '\n';
                buf[strlen(buf)] = 0;
                send(client,buf,strlen(buf),0);
        }
    }
    close(client);
    return NULL;
}