#include <k_stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "k_socket_include.h"

// 组播服务器，发送一个组播报文 224.0.0.0 - 239.255.255.25
int main()
{
	int listenSoc;
    struct sockaddr_in serAddr,cliAddr;
    socklen_t serLen;
    int w;
    char buf[128]={0};

    listenSoc=socket(AF_INET,SOCK_DGRAM,0);
    if(listenSoc==-1) K_MSG("socket error:",-1);

	serAddr.sin_family=AF_INET;
	serAddr.sin_port=htons(SERVER_PORT);
	//serAddr.sin_addr.s_addr=htons(INADDR_ANY);
    inet_pton(AF_INET,"224.0.0.1",&(serAddr.sin_addr));
	serLen=sizeof(serAddr);

    int i=0;
    while(1)
    {
        if(i>=5) break;
        i++;

        strcpy(buf,"hello this is mul info\n");
        w=sendto(listenSoc,buf,128,0,
                (struct sockaddr*)&serAddr,serLen);

        sleep(1);
    }

    close(listenSoc);
    return 0;
}