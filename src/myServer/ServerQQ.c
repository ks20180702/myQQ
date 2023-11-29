#include <k_stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <string.h>
#include <pthread.h>
#include <sys/epoll.h>
#include "k_socket_include.h"
#include "read.h"

//获取端口监听套接字
int listen_get()
{
	int listenSoc;
	listenSoc=socket(AF_INET,SOCK_STREAM,0);
	if(listenSoc==-1) K_MSG("socket ",-1);

	struct sockaddr_in serAddr;
	serAddr.sin_family=AF_INET;
	serAddr.sin_port=htons(SERVER_PORT);
	serAddr.sin_addr.s_addr=htons(INADDR_ANY);
	
	int addrLen=sizeof(serAddr);
	int bindRe=bind(listenSoc,(struct sockaddr*)&serAddr,addrLen);
	if(bindRe==-1) K_MSG("bind",-1);

	int lr=listen(listenSoc,5);
    if(lr==-1) K_MSG("listen",-1);

    return listenSoc;
}
int server_init()
{
    int listenSoc,clientSoc;
    listenSoc=listen_get();
    if(listenSoc ==-1) return -1;
}
int main()
{
    int listenSoc,clientSoc;
	struct sockaddr_in cliAddr;
	socklen_t cliLen;
	size_t r,w;
	char readBuf[128],writeBuf[128],overBuf[3]="\r\n",pathInfo[128]={0};
    int epollFd; 
    struct epoll_event epEv,epList[500];
    int epollReadyNum;


    epollFd=epoll_create(500);
    if(epollFd==-1) {perror("epoll error:");return -1;}

    epEv.data.fd=listenSoc;
    epEv.events=EPOLLIN;
    int rec=epoll_ctl(epollFd,EPOLL_CTL_ADD,listenSoc,&epEv);
    if(rec==-1) {perror("epoll_ctl error:");return -1;}

    while(1)
    {
        epollReadyNum=epoll_wait(epollFd,epList,500,-1);
        if(epollReadyNum<0) {perror("epoll_wait error:");return -1;}

        for(int i=0;i<epollReadyNum;i++)
        {
            if(epList[i].data.fd==listenSoc) //新连接到来
            {
                clientSoc=accept(listenSoc,(struct sockaddr*)&cliAddr,&cliLen);
                if(clientSoc==-1) {perror("accept error:");return -1;}

                //将新的客户端加入epoll
                epEv.data.fd=clientSoc;
                epEv.events=EPOLLIN;  //默认为水平触发(未读取完数据会一直通知)
                rec=epoll_ctl(epollFd,EPOLL_CTL_ADD,clientSoc,&epEv);
                if(rec==-1) {perror("epoll_ctl error:");return -1;}
            }
            else{  //接收到数据
                memset(readBuf,0,128);
                r=recv(epList[i].data.fd,readBuf,128,0);
                if(r<0)
                { 
                    rec=epoll_ctl(epollFd,EPOLL_CTL_DEL,epList[i].data.fd,&(epList[i]));
                    close(epList[i].data.fd);
                    perror("recv error:");
                    continue;
                }
                else if(r==0) //结束通信
                {  
                    rec=epoll_ctl(epollFd,EPOLL_CTL_DEL,epList[i].data.fd,&(epList[i]));
                    if(rec==-1) {perror("epoll_ctl del error:");return -1;}
                    close(epList[i].data.fd);
                    printf("fd = %d ,over \n",epList[i].data.fd);
                    continue;
                }
                else{ //接收到数据后的处理
                    char inputType[10];
                    recv(epList[i].data.fd,inputType,10,0);
                    if(strcmp(inputType,"path")==0)
                    {
                        memset(pathInfo,0,128);
                        strcat(pathInfo,readBuf);
                        int rs=showDir(readBuf,epList[i].data.fd);
                        if(rs==-1) send(epList[i].data.fd,"input path error",sizeof("input path error"),0);
                        send(epList[i].data.fd,overBuf,3,0); //自定义结束
                    }
                    else if(strcmp(inputType,"fileName")==0)
                    {
                        strcat(pathInfo,"/");
                        strcat(pathInfo,readBuf);
                        int rr=readFile(pathInfo,epList[i].data.fd);
                        if(rr==-1) send(epList[i].data.fd,"read file error",sizeof("read file error"),0);
                        send(epList[i].data.fd,overBuf,3,0); //自定义结束
                    }  
                }
            }
        }
    }
}