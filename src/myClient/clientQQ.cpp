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

                memset(buf,0,SEND_RECV_BUF_SIZE);
                r=recvfrom(_cliSoc,buf,SEND_RECV_BUF_SIZE,0,(struct sockaddr*)&_serAddr,&serLen);
                if(r<0) {strcpy(_errMsg,"recvfrom error "); return -1;}

                recv_cmd_part(buf,r);
                
            }
            else if(i==0) // i/o输入
            {
                FD_CLR(i,&currFdset);

                memset(inputDate,0,SEND_RECV_BUF_SIZE);
                scanf("%s",inputDate);

                // send_part(inputDate,strlen(inputDate),true);
                // send_part(inputDate,strlen(inputDate),false);
                send_part(testStr,n,true);
            }
        }
    }
}
int ClientQQ::send_part(char *sendStr,int n,bool isCmd)
{
    size_t w;
    char *sendTemp=sendStr;
    int nowNum=0,sendLen;
    
    if(isCmd==true)
    {
        w=sendto(_cliSoc,"KS_START",sizeof("KS_START"),0,(struct sockaddr*)&_serAddr,sizeof(_serAddr));
        if(w<0) {strcpy(_errMsg,"send error"); return -1;}
    }

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

    if(isCmd==true)
    {
        w=sendto(_cliSoc,"KS_END",sizeof("KS_END"),0,(struct sockaddr*)&_serAddr,sizeof(_serAddr));
        if(w<0) {strcpy(_errMsg,"send error"); return -1;}
    }

    return 0;
}
int ClientQQ::recv_cmd_part(char *buf,int readNum)
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
int ClientQQ::param_cmd_str(std::string cmdStr)
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