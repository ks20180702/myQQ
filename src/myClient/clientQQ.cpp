#include "include/clientQQ.h"
#include "k_socket_include.h"

ClientQQ::ClientQQ()
    :_cliSoc(-1){FD_ZERO(&_globalFdset);}
int ClientQQ::client_init(char *ipAddr)
{
    _cliSoc=socket(AF_INET,SOCK_DGRAM,0);
    if(_cliSoc==-1) {strcpy(_errMsg,"socket create "); return -1;}

    _serAddr.sin_family=AF_INET;
    _serAddr.sin_port=htons(SERVER_PORT);
    inet_pton(AF_INET,ipAddr,&(_serAddr.sin_addr));
    
    return 0;
}
int ClientQQ::select_init()
{
    FD_ZERO(&_globalFdset);
    FD_SET(0,&_globalFdset); //加入标准输入
    FD_SET(_cliSoc,&_globalFdset);
}
int ClientQQ::run(char *testStr,int n)
{
    fd_set currFdset;
    struct sockaddr_in serAddr;
    socklen_t serLen=sizeof(serAddr);
    char buf[SEND_RECV_BUF_SIZE]={0},inputDate[SEND_RECV_BUF_SIZE]={0};
    size_t r;

    int ndfsNum;
    int nfdsMax=_cliSoc; //其实客户端这里只有两个，且不会变，这样写只是为了看起来通用

    while(1)
    {
        socklen_t serLen=sizeof(_serAddr);
        currFdset=_globalFdset;
        
        if((ndfsNum=select(nfdsMax+1,&currFdset,NULL,NULL,NULL))<0) {perror("select error:");break;}
        if(ndfsNum==0) continue;

        for(int i=0;i<=nfdsMax;i++)
        {
            if(!FD_ISSET(i,&currFdset)) continue;

            if(i==_cliSoc) // 接收到服务端的数据
            {
                FD_CLR(i,&currFdset);

                r=recvfrom(_cliSoc,buf,SEND_RECV_BUF_SIZE,0,(struct sockaddr*)&_serAddr,&serLen);
                if(r==-1) {strcpy(_errMsg,"recvfrom error "); return -1;}
                write(1,"recv from : ",sizeof("recv from : "));
                write(1,buf,r);
                write(1,"\n",2);
            }
            else if(i==0) // i/o输入
            {
                FD_CLR(i,&currFdset);

                memset(inputDate,0,SEND_RECV_BUF_SIZE);
                scanf("%s",inputDate);

                // send_part(inputDate,strlen(inputDate));
                send_part(testStr,n);
            }
        }
    }
}
int ClientQQ::send_part(char *sendStr,int n)
{
    size_t w;
    char *sendTemp=sendStr;
    int nowNum=0,sendLen;
    while(nowNum<n)
    {
        if((n-nowNum)>=SEND_RECV_BUF_SIZE)
        {
            sendLen=SEND_RECV_BUF_SIZE;
        }
        else{
            sendLen=n-nowNum;
        }
        w=sendto(_cliSoc,sendTemp+nowNum,sendLen,0,(struct sockaddr*)&_serAddr,sizeof(_serAddr));
        if(w<0) {strcpy(_errMsg,"send error"); return -1;}
        nowNum+=w;
    }
    return 0;
}
char *ClientQQ::get_error()
{
    return (char*)_errMsg;
}

//显示上一个错误的错误详情
void ClientQQ::show_error_detail()
{
    perror(_errMsg);
}

int& ClientQQ::get_socket()
{
    return _cliSoc;
}

ClientQQ::~ClientQQ()
{
    close(_cliSoc);
}