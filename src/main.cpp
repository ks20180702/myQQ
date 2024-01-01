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
        // logInfo.add_login_user(myUser);
        myTest.push_back(myUser);
    }
    
    char vecChar[sizeof(decltype(myTest)::value_type)*myTest.size()]; 
    for(std::vector<CUser>::iterator it=myTest.begin();it!=myTest.end();it++)
    {
        memcpy(vecChar+sizeof(decltype(myTest)::value_type)*(it-myTest.begin()),&(*it),sizeof(*it));
    }
    // char cmdChar[sizeof(logInfo)];
    // memcpy(cmdChar,&logInfo,sizeof(logInfo));

    char cmdChar[sizeof(logInfo)+sizeof(vecChar)];
    memcpy(cmdChar,&logInfo,sizeof(logInfo));
    memcpy(cmdChar+sizeof(logInfo),vecChar,sizeof(vecChar));

    std::cout<<sizeof(decltype(myTest)::value_type)*myTest.size()<<std::endl;
    std::cout<<sizeof(cmdChar)<<std::endl;

    myCli.run(cmdChar,sizeof(cmdChar));
    myCli.show_error_detail();

    std::cout<<"main over"<<std::endl;
}


