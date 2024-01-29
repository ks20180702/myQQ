#include <iostream>

using namespace std;
#include "user.h"
#include "otlUse.h"
#include "cmdBase.h"
#include "loginCmd.h"
#include "userChangeCmd.h"
#include "msg.h"
#include "dataCmd.h"

#include "userNotRecvMsg.h"
#include "serverQQ.h"
using namespace std;

/*
    这里有个好奇该的问题，暂时没找到原因
    将run(注释的语句)放在main中，则recvfrom接收不到数据，
    直接使用run则可以成功接收数据
*/
int main()
{
    CUser myUser(1,(char*)"123456",(char*)"123456",(char*)"ks",23,"","2023-11-29 19:32:00");
    CLoginCmd logInfo(myUser);

    std::ostringstream ss;
    cereal::JSONOutputArchive archive(ss);
    archive(cereal::make_nvp("logInfo._childCmdType", logInfo._childCmdType),cereal::make_nvp("logInfo", logInfo));
    // std::cout<<ss.str()<<std::endl;

	std::string testStr=ss.str()+"\n}";
    std::cout<<testStr<<std::endl;
    
    CLoginCmd logInfo2;
    CmdBase::CmdType childCmdType;

	std::istringstream iss(testStr);
	cereal::JSONInputArchive archive1(iss);
	archive1(cereal::make_nvp("logInfo._childCmdType", childCmdType));
    std::cout<<childCmdType<<std::endl;
	archive1(cereal::make_nvp("logInfo", logInfo2));
    // std::cout<<logInfo.childCmdType<<std::endl;
	(logInfo2.get_login_user()).print();

    return 0;
    
    std::cout<<"main +++"<<std::endl;

    CServerQQ mySer;

    mySer.connect_db();

    if(mySer.server_bind()==-1) mySer.show_error_detail();

    mySer.run();
}


