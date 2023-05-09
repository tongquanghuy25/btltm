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
    if(argc != 3){
        printf("Sai tham so !\n");
        return 0;
    }

    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    char buf[256];
    struct sockaddr_in sender_addr;
    int sender_addr_len = sizeof(sender_addr);

    int ret = recvfrom(receiver, buf, sizeof(buf), 0,
            (struct sockaddr *)&sender_addr, &sender_addr_len);
    if (ret < sizeof(buf))
        buf[ret] = 0;
    printf("%s \n",buf);

    char *data_file = argv[2];

    FILE *f = fopen(data_file,"w");


    while (1)
    {
        ret = recvfrom(receiver, buf, sizeof(buf), 0,
            (struct sockaddr *)&sender_addr, &sender_addr_len);
        fprintf(f,"%s",buf);

        printf("%d bytes received %s: %s\n", ret, 
            inet_ntoa(sender_addr.sin_addr), buf);
        

        if(strncmp(buf,"exit",4)) break;

        printf("%d bytes received %s: %s\n", ret, 
            inet_ntoa(sender_addr.sin_addr), buf);
        
        
        
    }
    fclose(f);
    return 0;
}