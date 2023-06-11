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

int main(){
    //khoi tao socket cho ket noi
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);

    if(bind(listener,(struct sockaddr *)&addr,sizeof(addr))){
        printf("binding failed !");
        return 1;
    }

    if(listen(listener,5)){
        printf("listening failed !");
        return 1;
    }

    int num_threads = 8;
    char buf[256];
    pthread_t thread_id;

    for(int i=0; i<num_threads; i++){
        int ret = pthread_create(&thread_id, NULL, thread_proc, &listener);
        if (ret != 0){
            printf("Could not create new thread.\n");
            sched_yield();
        }

        pthread_join(thread_id, NULL);
        return 0;
    }

    return 0;
}

void *thread_proc(void *para){
    char buf[256];
    int listener = *(int *)para;
    int client = accept(listener, NULL, NULL);
    printf("New client %d accepted in thread %ld in process %d:\n", client, pthread_self(), getpid());
    while (1)
    {

        // Chờ dữ liệu từ client
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0){
            close(client);
            return NULL;
        }
            

        // Xử lý dữ liệu, trả lại kết quả cho client
        buf[ret] = 0;
        printf("Received from client %d: %s\n", client, buf);
        char *msg = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Xin chao cac ban</h1></body></html>\n";
        send(client, msg, strlen(msg), 0);

    }
    
    close(client);
    return NULL;
}
