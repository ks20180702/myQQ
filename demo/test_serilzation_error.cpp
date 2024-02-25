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
#include "clientQQ.h"
using namespace std;

#include <typeinfo>

/*
    测试手动序列化对象，发现会出现错误，服务器端将字节流转成对象后会出现核心错误
*/

int main()
{
    std::cout<<"main +++"<<std::endl;

    ClientQQ myCli;
    myCli.client_init("192.168.47.136");
    myCli.select_init();

    CUser myUser(10,(char*)"123456",(char*)"123456",(char*)"ks",23,"","2023-11-29 19:32:00");
    CLoginCmd logInfo(myUser);

    std::vector<CUser> myTest;
    for(int i=0;i<10;i++)
    {
        CUser myUser(i,(char*)"123456",(char*)"123456",(char*)"ks",23,"","2023-11-29 19:32:00");
        myTest.push_back(myUser);
    }
    logInfo.set_friend_lists(myTest);

    char vecChar[logInfo.get_obj_sizeof()]; 
    // logInfo.obj_to_char(vecChar);
    memcpy(vecChar,&logInfo,sizeof(logInfo));

    for(std::vector<CUser>::iterator it=myTest.begin();it!=myTest.end();it++)
    {
        memcpy(vecChar+sizeof(logInfo)+sizeof(CUser)*(it-myTest.begin()),&(*it),sizeof(*it));
    }
    std::cout<<sizeof(vecChar)<<std::endl;

    // // memcpy(&logInfo2,vecChar,sizeof(logInfo2));
    // memcpy(&logInfo2,cmdStr.c_str(),sizeof(logInfo2));
    // (logInfo2.get_login_user()).print();

    // logInfo2=logInfo;
    // std::vector<CUser> friendLists;
    // CUser myTT;
    // for(int i=0;i<10;i++)
    // {
    //     memcpy(&myTT,cmdStr.c_str()+sizeof(CLoginCmd)+sizeof(myTT)*i,sizeof(myTT));
    //     // memcpy(&myTT,vecChar+sizeof(CLoginCmd)+sizeof(myTT)*i,sizeof(myTT));
    //     myTT.print();
    //     friendLists.push_back(myTT);
    // }
    // std::cout<<endl;
    // std::cout<<typeid(myTest).name()<<std::endl;

    myCli.run(vecChar,sizeof(vecChar));
    myCli.show_error_detail();

    std::cout<<"main over"<<std::endl;
}


