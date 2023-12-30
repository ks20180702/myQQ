#include "./include/serverQQ.h"


CServerQQ::CServerQQ()
{
}

int CServerQQ::server_bind()
{
    int bindRe;
    struct sockaddr_in serAddr;
    int serLen;

    _serSoc=socket(AF_INET,SOCK_DGRAM,0);
    if(_serSoc==-1) {strcpy(_errMsg,"socket error "); return -1;}

    serAddr.sin_family=AF_INET;
    serAddr.sin_port=htons(SERVER_PORT);
    serAddr.sin_addr.s_addr=htons(INADDR_ANY);
    //inet_pton(AF_INET,"192.168.47.132",&(serAddr.sin_addr));
    serLen=sizeof(serAddr);

    bindRe=bind(_serSoc,(struct sockaddr *)&serAddr,serLen);
    if(bindRe==-1) {strcpy(_errMsg,"bind error "); return -1;}
    
    return 0;
}
int CServerQQ::run()
{
    struct sockaddr_in cliAddr;
    socklen_t cliLen;
    size_t r,w;
    char buf[1024*10]={0};
    while(1)
    {
        r=recvfrom(_serSoc,buf,1024*10,0,
                (struct sockaddr*)&cliAddr,&cliLen);
        if(r<0) {strcpy(_errMsg,"recvfrom error"); return -1;}

        write(1,buf,r);
        write(1,"\n",2);
        w=sendto(_serSoc,buf,r,0,
                (struct sockaddr*)&cliAddr,cliLen);
        if(w<0) {strcpy(_errMsg,"sendto error"); return -1;}
    }
}
int& CServerQQ::get_socket()
{
    return _serSoc;
}
char *CServerQQ::get_error()
{
    return (char*)_errMsg;
}
void CServerQQ::show_error_detail()
{
    perror(_errMsg);
}
CServerQQ::~CServerQQ()
{
    close(_serSoc);
}