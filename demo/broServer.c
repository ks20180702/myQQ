#include <k_stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "k_socket_include.h"

int main()
{
	int listenSoc;
    struct sockaddr_in serAddr,cliAddr;
    socklen_t serLen,cliLen;
    int lr,bindRe;  //记录一些函数的返回值，-1时表示出错
    size_t r,w;
    char buf[128]={0};

    listenSoc=socket(AF_INET,SOCK_DGRAM,0);
    if(listenSoc==-1) K_MSG("socket error:",-1);

	serAddr.sin_family=AF_INET;
	serAddr.sin_port=htons(SERVER_PORT);
	serAddr.sin_addr.s_addr=htons(INADDR_ANY);
    //inet_pton(AF_INET,"192.168.47.132",&(serAddr.sin_addr));
	serLen=sizeof(serAddr);
	bindRe=bind(listenSoc,(struct sockaddr*)&serAddr,serLen);
	if(bindRe==-1) K_MSG("bind",-1);

    r=recvfrom(listenSoc,buf,128,0,
            (struct sockaddr*)&cliAddr,&cliLen);
    printf("%s",buf);

    strcpy(buf,"hello this is server\n");
    w=sendto(listenSoc,buf,128,0,
            (struct sockaddr*)&cliAddr,cliLen);
    
    return 0;
}