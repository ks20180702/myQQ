#include "./include/serverQQ.h"

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
CServerQQ::CServerQQ():_cmdOtlUse(),_nowUseCmdObj(nullptr),_factoryCreater(nullptr){
    _clientCmdStrMap.clear();
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
void* CServerQQ::pthread_fun(void *arg)
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
    char buf[1024]={0};
    std::string returnCmdJosnStr,cliUrl;
    map<string,string>::iterator cliIt;

    cliLen=sizeof(cliAddr);
    while(1)
    {
        memset(&cliAddr,0,sizeof(cliAddr));
        while(1)
        {
            memset(buf,0,1024);

            r=recvfrom(_serSoc,buf,1024,0,
                    (struct sockaddr*)&cliAddr,&cliLen);
            if(r<0) {strcpy(_errMsg,"recvfrom error"); return ;}
            // std::cout<<buf<<std::endl;
            
            cliUrl=inet_ntoa(cliAddr.sin_addr);
            cliIt=_clientCmdStrMap.find(cliUrl);
            
            //第一次接收到某个客户端的数据,且为指令开始标记
            if(cliIt==_clientCmdStrMap.end())
            {
                if(strcmp(buf,"KS_START")==0)
                {
                    _clientCmdStrMap.insert(map<string,string>::value_type(cliUrl,""));
                    std::cout<<"ip = "<<cliUrl<<" is start "<<std::endl;
                }
            }
            //非第一次且不为指令结束标记，那就加起来
            else{
                if(strcmp(buf,"KS_END")==0)
                {                    
                    //执行接收到的完整的指令，执行完毕后，清除掉指令
                    param_cmd_str(cliIt->second,returnCmdJosnStr);
                    _clientCmdStrMap.erase(cliIt);
                    send_part((char *)(cmdJosnStr.c_str()),cmdJosnStr.length(),cliAddr);

                    std::cout<<"ip = "<<cliUrl<<" is over "<<std::endl;
                    break;
                }
                else{
                    cliIt->second+=std::string(buf,r);
                }
            }
            std::cout<<"s_addr = "<<inet_ntoa(cliAddr.sin_addr)<<std::endl;
        }

    }
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

                if(pthread_create(&myThread,&myThreadAttr,pthread_fun,this))
                {
                    strcpy(_errMsg,"pthread_creat error "); return -1;
                } 
            } 
        }
    }
}

int CServerQQ::param_cmd_str(std::string cmdStr,std::string &returnCmdJosnStr)
{
    std::shared_ptr<CmdBase> useCmdObj;

    CmdBase::CmdType childCmdType;

    // std::cout<<cmdStr+"\n}"<<std::endl;
	std::istringstream iss(cmdStr+"\n}");
	cereal::JSONInputArchive jsonIA(iss);
	jsonIA(cereal::make_nvp("_childCmdType", childCmdType));

    useCmdObj=shared_ptr<CmdBase>(_factoryCreater->create_cmd_ptr(childCmdType));
    useCmdObj->reload_recv_obj_by_json(jsonIA);

    useCmdObj->do_command(_cmdOtlUse);
    
    // useCmdObj->show_do_command_info();

    returnCmdJosnStr=useCmdObj->get_command_obj_json();

    return 0;
}
void CServerQQ::Test()
{  
    // _cmdOtlUse.change_request_friend_type(1,2,4);


    // // 不报错，但是修改无效
    // CUser user((char*)"121212",(char*)"123456",(char*)"add_24",23);
    // std::cout<<_cmdOtlUse.set_user_id_by_account(user)<<std::endl;
    // user.print();

    // user.set_user_info(user.get_id(),user.get_account(),user.get_password(),
    // (char*)"add====",23,"","2024-1-16 19:32:00");
    // std::cout<<_cmdOtlUse.change_user(user)<<std::endl;
    // std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;
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
        write(1,sendTemp+nowNum,sendLen);

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
    return (char*)_errMsg;
}
void CServerQQ::show_error_detail()
{
    perror(_errMsg);
}
CServerQQ::~CServerQQ()
{
    close(_serSoc);
    _clientCmdStrMap.clear();
}