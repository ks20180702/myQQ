#include <k_stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "k_socket_include.h"

int client_read_deal(int clientSoc,fd_set *readFdSetPtr,fd_set *currReadFdSetPtr,int *inputTypePtr)
{
    char buf[128]={0};
    int r;

    FD_CLR(clientSoc,currReadFdSetPtr);
    while((r=recv(clientSoc,buf,128,0))>0)
    {
        if(strcmp(buf,"\r\n")==0) break; //自定义结束
        write(1,buf,r);  //do something
        memset(buf,0,128);
    }
    if(r<0) {perror("recv error:");close(clientSoc);return -1;}
    else if(r==0) //通信结束
    {
        close(clientSoc);
        write(1,"connect over.\n",sizeof("connect over.\n"));
        return 0;
    }

    if((*inputTypePtr)==0)
    {
        *inputTypePtr=1;
        printf("\nplease input fileName:\n");
    }
    else{
        *inputTypePtr=0;
        printf("\nplease input path:\n");
    }
    FD_SET(0,readFdSetPtr);
    
    return 0;
}
int input_deal(int clientSoc,fd_set *readFdSetPtr,fd_set *currReadFdSetPtr,int inputType)
{
    char inputInfo[128]={0}; //输入的路径
    int w;

    FD_CLR(0,currReadFdSetPtr);
    FD_CLR(0,readFdSetPtr);

    memset(inputInfo,0,128);
    scanf("%s",inputInfo);
    w=send(clientSoc,inputInfo,128,0);
    if(w<0) {perror("send error:");return -1;}

    if(inputType==0) send(clientSoc,"path",5,0);
    else send(clientSoc,"fileName",9,0);

    return 0;
}
int main()
{
	int clientSoc;
    struct sockaddr_in cliAddr;
    socklen_t cliLen;
    int cr,sr;  //记录一些函数的返回值，-1时表示出错
    size_t r,w;
    fd_set readFdSet,currReadFdSet; //一个记录当前，一个用来记录修改的
    int nfdsMax,ndfsNum; //当前加入到select的句柄最大值
    int inputType=0; //0表示输入为路径，1表示输入为需读取的文件名

    clientSoc=socket(AF_INET,SOCK_STREAM,0);
    if(clientSoc==-1) K_MSG("socket error:",-1);

    cliAddr.sin_family=AF_INET;
    cliAddr.sin_port=htons(SERVER_PORT);
    inet_pton(AF_INET,"192.168.47.132",&(cliAddr.sin_addr));
    cliLen=sizeof(cliAddr);
    cr=connect(clientSoc,(struct sockaddr*)&cliAddr,cliLen);
    if(cr==-1) K_MSG("connect error:",-1);

    //读通知集合增加
    FD_ZERO(&readFdSet);
    FD_SET(0,&readFdSet); //加入标志输入
    FD_SET(clientSoc,&readFdSet);
    nfdsMax=clientSoc;
    printf("please input path:\n");
    //使用select进行通知‘数据到来’和‘i/o输入’
    while(1)
    {
        currReadFdSet=readFdSet;
        if((ndfsNum=select(nfdsMax+1,&currReadFdSet,NULL,NULL,NULL))<0) {perror("select error:");break;}
        if(ndfsNum==0) continue;

        for(int i=0;i<=nfdsMax;i++)
        {
            if(!FD_ISSET(i,&currReadFdSet)) continue;

            if(i==clientSoc) // 接收到服务端的数据
            {
                int crd=client_read_deal(i,&readFdSet,&currReadFdSet,&inputType);
            }
            else if(i==0) // i/o输入
            {
                int ir=input_deal(clientSoc,&readFdSet,&currReadFdSet,inputType);
            }
        }
    }
    close(clientSoc);

    return 0;
}