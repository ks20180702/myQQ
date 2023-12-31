#include "./include/serverQQ.h"


/*
    这里有个好奇怪的问题，第一次启动ser时，客户端发送的KS_START，服务端可以接收到，
    但是发送不到客户端(客户端接收不到第一次发送的数据)
*/
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
        memset(buf,0,1024*10);
        r=recvfrom(_serSoc,buf,1024*10,0,
                (struct sockaddr*)&cliAddr,&cliLen);
        if(r<0) {strcpy(_errMsg,"recvfrom error"); return -1;}

        // std::cout<<buf<<" "<<r<<std::endl;
        recv_cmd_part(buf,r);

        w=sendto(_serSoc,buf,r,0,
                (struct sockaddr*)&cliAddr,cliLen);
        if(w<0) {strcpy(_errMsg,"sendto error"); return -1;}
    }
}
int CServerQQ::recv_cmd_part(char *buf,int readNum)
{
    //是否开始接收，当接收到开始标志(KS_START)表示开始接收整条语句
    static bool cmdStrOver=false;
    static std::string tempStr="";

    if(cmdStrOver==true)
    {
        if (strcmp(buf,"KS_END")==0)
        {
            std::cout<<"tempStr cmd =  "<<tempStr.length()<<std::endl;
            param_cmd_str(tempStr);

            cmdStrOver=false;
            tempStr="";
        }
        else
        {
            tempStr+=std::string(buf,readNum);
        }
    }
    else
    {
        // 接收结构体
        if(strcmp(buf,"KS_START")==0)
        {
            cmdStrOver=true;
            tempStr="";
        }
        else
        {
            std::cout<<"recv from : "<<buf<<std::endl;
        }
    }
    return 0;
}

int CServerQQ::param_cmd_str(std::string cmdStr)
{
    CLoginCmd *logInfo=(CLoginCmd *)new char [cmdStr.length()];;
    memcpy(logInfo,cmdStr.c_str(),cmdStr.length());


    std::vector<CUser> friendLists=logInfo->get_friend_lists();
    for(std::vector<CUser>::iterator it=friendLists.begin();it!=friendLists.end();it++)
    {
        (*it).print();
    }
    (logInfo->get_login_user()).print();

    return 0;
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