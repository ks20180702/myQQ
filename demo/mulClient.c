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
    int clientSoc;
    struct sockaddr_in cliAddr,serAddr;
    socklen_t serLen;
    int cr,sr,setOptRe;  //记录一些函数的返回值，-1时表示出错
    size_t r,w;
    char buf[128]={0};
    struct ip_mreq mreq;

    clientSoc=socket(AF_INET,SOCK_DGRAM,0);
    if(clientSoc==-1) K_MSG("socket error:",-1);

	serAddr.sin_family=AF_INET;
	serAddr.sin_port=htons(SERVER_PORT);
	serAddr.sin_addr.s_addr=htons(INADDR_ANY);
    //inet_pton(AF_INET,"192.168.47.132",&(serAddr.sin_addr));
	serLen=sizeof(serAddr);
	int bindRe=bind(clientSoc,(struct sockaddr*)&serAddr,serLen);
    if(bindRe==-1) K_MSG("bind",-1);

    //加入组播组
    mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.1"); //mcast_ip
    mreq.imr_interface.s_addr = inet_addr("192.168.47.132"); //本机接口，若由多个ip，可使用INETADDR_ANY
    setOptRe=setsockopt(clientSoc,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
    if(setOptRe==-1) K_MSG("setsockopt",-1);

    while(1)
    {
        memset(buf,0,128);
        r=recvfrom(clientSoc,buf,128,0,
                (struct sockaddr*)&cliAddr,&serLen);
        if(r==-1) K_MSG("sendto error:",-1); 
        printf("%s",buf);
    }

    setOptRe=setsockopt(clientSoc,IPPROTO_IP,IP_DROP_MEMBERSHIP,&mreq,sizeof(mreq));
    if(setOptRe==-1) K_MSG("setsockopt",-1);
    close(clientSoc);
    return 0;
}