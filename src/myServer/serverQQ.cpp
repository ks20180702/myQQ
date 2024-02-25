#include "./include/serverQQ.h"

#include "msg.h"
#include <vector>
/*
    这里有个好奇怪的问题，第一次启动ser时，客户端发送的KS_START，服务端可以接收到，
    但是发送不到客户端(客户端接收不到第一次发送的数据)
*/
CServerQQ::CServerQQ():_cmdOtlUse(),_nowUseCmdObj(nullptr),_factoryCreater(nullptr){}

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
    char buf[1024]={0};
    std::string cmdJosnStr;
    while(1)
    {
        memset(&cliAddr,0,sizeof(cliAddr));
        while(1)
        {
            memset(buf,0,1024);
            r=recvfrom(_serSoc,buf,1024,0,
                    (struct sockaddr*)&cliAddr,&cliLen);
            if(r<0) {strcpy(_errMsg,"recvfrom error"); return -1;}

            recv_cmd_part(buf,r);

            if(strcmp(buf,"KS_END")==0)
            {
                std::cout<<"[over] recv KS_END now over"<<std::endl;
                break;
            }
        }
        //
        // std::cout<<"_serSoc = "<<_serSoc<<std::endl;
        // std::cout<<"s_addr = "<<inet_ntoa(cliAddr.sin_addr)<<std::endl;
        // printf("port=%d\n",ntohs(cliAddr.sin_port));

        cmdJosnStr=_nowUseCmdObj->get_command_obj_json();

        send_part((char *)(cmdJosnStr.c_str()),cmdJosnStr.length(),cliAddr);
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
    CmdBase::CmdType childCmdType;

	std::istringstream iss(cmdStr+"\n}");
    std::cout<<cmdStr+"\n}"<<std::endl;

	cereal::JSONInputArchive jsonIA(iss);
	jsonIA(cereal::make_nvp("_childCmdType", childCmdType));

    _nowUseCmdObj=shared_ptr<CmdBase>(_factoryCreater->create_cmd_ptr(childCmdType));
    _nowUseCmdObj->reload_recv_obj_by_json(jsonIA);
    // if(nullptr!= _nowUseCmdObj)
    // {
    //     _nowUseCmdObj=nullptr;
    // }
    // if(CmdBase::LOGIN_CMD== childCmdType)
    // {
    //     CLoginCmd logInfo;
    //     archive(cereal::make_nvp("logInfo", logInfo));

    //     _nowUseCmdObj=std::make_shared<CLoginCmd>(logInfo);
    // }
    // else if(CmdBase::USER_CHANGE_CMD== childCmdType)
    // {
    //     CUserChangeCmd logInfo;
    //     archive(cereal::make_nvp("logInfo", logInfo));

    //     _nowUseCmdObj=std::make_shared<CUserChangeCmd>(logInfo);
    // }
    // else if(CmdBase::FRIEND_SHIP_CHANGE_CMD== childCmdType)
    // {
    //     CFriendshipChangeCmd logInfo;
    //     archive(cereal::make_nvp("logInfo", logInfo));

    //     _nowUseCmdObj=std::make_shared<CFriendshipChangeCmd>(logInfo);
    // }

    _nowUseCmdObj->do_command(_cmdOtlUse);
    
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
}