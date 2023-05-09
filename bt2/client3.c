#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if(argc != 4 ){
        printf("Sai tham so !\n");
        return 0;
    }

    char buf[256];

    

    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));


    char *file_txt = argv[3];
    sendto(sender,argv[3],sizeof(argv[3]),0,(struct sockaddr *)&addr, sizeof(addr));

    FILE *f = fopen(file_txt,"r");
    
    while (1)
    {
        if(fread(buf,1,sizeof(buf),f) <= 0)
            break;
        int ret = sendto(sender, buf, strlen(buf), 0,
            (struct sockaddr *)&addr, sizeof(addr));
        printf("%d bytes sent.\n", ret);
    }
    sendto(sender,"exit",5,0,(struct sockaddr *)&addr,sizeof(addr));
    return 0;
}