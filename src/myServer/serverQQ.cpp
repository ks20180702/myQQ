#include "./include/serverQQ.h"

#include "heartRequestCmd.h"

#include "msg.h"
#include <vector>

#include <sys/wait.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>

/*
    这里有个好奇怪的问题，第一次启动ser时，客户端发送的KS_START，服务端可以接收到，
    但是发送不到客户端(客户端接收不到第一次发送的数据)
*/
CServerQQ::CServerQQ():_cmdOtlUse(),_factoryCreater(nullptr){
    _clientCmdStrMap.clear();
    _onlineClientMap.clear();
}

int CServerQQ::connect_db(char *connectStr)
{
    if(_cmdOtlUse.olt_init(connectStr)==-1) 
    {
        std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;
        return -1;
    }
    else{
        std::cout<<"connect db succeed"<<std::endl;
    }

    _factoryCreater=std::make_shared<CmdCreateFactory>();
}

int CServerQQ::server_bind()
{
    struct sockaddr_in serAddr;
    int serLen;
    int bindRe;

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
    int kdpfd,nfds,n;
    struct epoll_event ev;
    struct epoll_event eventsList[MAX_EPOLL_SIZE];

    pthread_t myThread;
    pthread_attr_t myThreadAttr;

    /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */
    kdpfd = epoll_create(MAX_EPOLL_SIZE);

    ev.events = EPOLLIN | EPOLLET; //加入连接时提醒
    ev.data.fd = _serSoc;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, _serSoc, &ev) < 0) 
    {
        strcpy(_errMsg,"epoll_ctl error "); return -1;
    }
    while (1) 
    {
        /* 等待有事件发生 */
        nfds = epoll_wait(kdpfd, eventsList, MAX_EPOLL_SIZE, -1);
        if (nfds == -1)
        {
            strcpy(_errMsg,"epoll_wait error "); return -1;
        }
        /* 处理所有事件 */
        for (n = 0; n < nfds; ++n)
        {
            if (eventsList[n].data.fd == _serSoc) 
            {
                /*初始化属性值，均设为默认值*/
                pthread_attr_init(&myThreadAttr);
                pthread_attr_setscope(&myThreadAttr, PTHREAD_SCOPE_SYSTEM);
                /*  设置线程为分离属性*/ 
                pthread_attr_setdetachstate(&myThreadAttr, PTHREAD_CREATE_DETACHED);

                if(pthread_create(&myThread,&myThreadAttr,PthreadFun,this))
                {
                    strcpy(_errMsg,"pthread_creat error "); return -1;
                } 
            } 
        }
    }
}

void* CServerQQ::PthreadFun(void *arg)
{
    CServerQQ *thiz=static_cast<CServerQQ *>(arg);

    pthread_t id = pthread_self();

    thiz->pthread_recv_and_send_msg();
    std::cout<<thiz->get_error()<<std::endl;

    pthread_exit(NULL);

    return NULL;
}
void CServerQQ::pthread_recv_and_send_msg()
{
    struct sockaddr_in cliAddr;
    socklen_t cliLen;
    size_t r,w;
    char buf[SEND_RECV_BUF_SIZE]={0};
    std::string returnCmdJosnStr,cliUrl;
    map<string,string>::iterator cliIt;
    int paramReturnNum=-1;

    cliLen=sizeof(cliAddr);

    memset(&cliAddr,0,sizeof(cliAddr));
    while(1)
    {
        memset(buf,0,SEND_RECV_BUF_SIZE);

        r=recvfrom(_serSoc,buf,SEND_RECV_BUF_SIZE,0,
                (struct sockaddr*)&cliAddr,&cliLen);
        if(r<0) {strcpy(_errMsg,"recvfrom error"); return ;}

        // 在一次接收和发送中，可以用ip+端口确定唯一主机。(一次接收中，端口并不会变)
        cliUrl=inet_ntoa(cliAddr.sin_addr)+std::string("_")+std::to_string(ntohs(cliAddr.sin_port));
        cliIt=_clientCmdStrMap.find(cliUrl);
        
        //第一次接收到某个客户端的数据,且为指令开始标记
        if(cliIt==_clientCmdStrMap.end())
        {
            if(strcmp(buf,"KS_START")==0)
            {
                _clientCmdStrMap.insert(map<string,string>::value_type(cliUrl,""));
                std::cout<<"recv ip = "<<cliUrl<<" is start "<<std::endl;
            }
        }
        //非第一次且不为指令结束标记，那就加起来
        else{
            if(strcmp(buf,"KS_END")==0)
            {                    
                //执行接收到的完整的指令，执行完毕后，清除掉指令
                paramReturnNum=param_cmd_str(cliIt->second,returnCmdJosnStr,cliAddr);
                _clientCmdStrMap.erase(cliIt);

                //执行结果0：返回客户端。1：不需要回传给客户端，-1：发生错误。
                if(paramReturnNum==0)
                {
                    send_part((char *)(returnCmdJosnStr.c_str()),returnCmdJosnStr.length(),cliAddr);
                }
                break;
            }
            else{cliIt->second+=std::string(buf,r);}
        }
        // std::cout<<"s_addr = "<<inet_ntoa(cliAddr.sin_addr)<<std::endl;
    }
}

int CServerQQ::param_cmd_str(std::string cmdStr,std::string &returnCmdJosnStr,struct sockaddr_in &cliAddr)
{
    // std::cout<<cmdStr+CMD_STR_ADD<<std::endl;

    //设置childCmdType
    CmdBase::CmdType childCmdType;
	std::istringstream istrStream(cmdStr+CMD_STR_ADD);
    std::shared_ptr<CmdBase> useCmdObj;

    try
    {
        cereal::JSONInputArchive jsonIA(istrStream);
	    jsonIA(cereal::make_nvp("_childCmdType", childCmdType));
        
        useCmdObj=shared_ptr<CmdBase>(_factoryCreater->create_cmd_ptr(childCmdType));
        useCmdObj->reload_recv_obj_by_json(jsonIA);
    }
    catch(const std::exception& e)
    {
        strcpy(_errMsg,"[E]  string to obj have a error"); 
        std::cerr << e.what() << '\n';
        return -1;
    }


    CmdBase::DoCommandReturnType doCommandRetrun;
    doCommandRetrun=useCmdObj->do_command(_cmdOtlUse);
    if(CmdBase::NO_SEND_CMD==doCommandRetrun) return 1;
    
    returnCmdJosnStr=useCmdObj->get_command_obj_json();
    return 0;
}
void CServerQQ::Test()
{  
    CUser recvUser("123456","123456","",0); //这个用户的id其实就是1
    CMsg testMsg(2,1,"","");
    CDataMsgCmd dataMsgCmd(recvUser,testMsg);
    dataMsgCmd.set_msg_request_type(CDataMsgCmd::MSG_CONFIRM);

    dataMsgCmd.do_command(_cmdOtlUse);

    dataMsgCmd.show_do_command_info();
}

int CServerQQ::send_part(char *sendStr,int n,sockaddr_in &cliAddr)
{
    size_t w;
    char *sendTemp=sendStr;
    int nowNum=0,sendLen;

    w=sendto(_serSoc,"KS_START",sizeof("KS_START"),0,(struct sockaddr*)&cliAddr,sizeof(cliAddr));
    if(w<0) {strcpy(_errMsg,"send error"); return -1;}

    while(nowNum<n)
    {
        if((n-nowNum)>=SEND_RECV_BUF_SIZE)
        {
            sendLen=SEND_RECV_BUF_SIZE;
        }
        else{
            sendLen=n-nowNum;
        }
        w=sendto(_serSoc,sendTemp+nowNum,sendLen,0,(struct sockaddr*)&cliAddr,sizeof(cliAddr));
        // write(1,sendTemp+nowNum,sendLen);

        if(w<0) {strcpy(_errMsg,"send error"); return -1;}

        nowNum+=w;
    }

    w=sendto(_serSoc,"KS_END",sizeof("KS_END"),0,(struct sockaddr*)&cliAddr,sizeof(cliAddr));
    if(w<0) {strcpy(_errMsg,"send error"); return -1;}

    return 0;
}

int& CServerQQ::get_socket()
{
    return _serSoc;
}
char *CServerQQ::get_error()
{
    std::string tempErrMsg=std::string(_errMsg);
    memset(_errMsg,0,128);
    return (char*)tempErrMsg.c_str();
}
void CServerQQ::show_error_detail()
{
    perror(_errMsg);
}
CServerQQ::~CServerQQ()
{
    close(_serSoc);
    _clientCmdStrMap.clear();
    _onlineClientMap.clear();
}