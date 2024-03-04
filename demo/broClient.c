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
    struct sockaddr_in cliAddr,serAddr,serAddrLis;
    socklen_t serLen;
    int cr,sr,setOptRe;  //记录一些函数的返回值，-1时表示出错
    size_t r,w;
    char buf[128]={0};
    int on=1;

    clientSoc=socket(AF_INET,SOCK_DGRAM,0);
    if(clientSoc==-1) K_MSG("socket error:",-1);

    serAddr.sin_family=AF_INET;
    serAddr.sin_port=htons(SERVER_PORT);
    inet_pton(AF_INET,"192.168.47.135",&(serAddr.sin_addr));
    // inet_pton(AF_INET,"192.168.47.255",&(serAddr.sin_addr));
    serLen=sizeof(serAddr);

    //运行使用广播地址(默认创建的socket是不允许的)
    setOptRe=setsockopt(clientSoc,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));
    if(setOptRe==-1) K_MSG("setsockopt",-1);

    strcpy(buf,"hello this is client\n");
    w=sendto(clientSoc,buf,128,0,
            (struct sockaddr*)&serAddr,serLen);
    if(w==-1) K_MSG("sendto error:",-1);

    memset(buf,0,128);
    r=recvfrom(clientSoc,buf,128,0,
            (struct sockaddr*)&serAddr,&serLen);
    if(r==-1) K_MSG("sendto error:",-1); 
    printf("%s",buf);
}