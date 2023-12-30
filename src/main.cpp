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
    // (loginCmd.get_login_user()).print();
    
    char cmdChar[sizeof(logInfo)];
    memcpy(cmdChar,&logInfo,sizeof(logInfo));

    for(int i=0;i<sizeof(logInfo);i++)
    {
        printf("%x",cmdChar[i]);
    }
    printf("\n");
    std::cout<<sizeof(logInfo)<<std::endl;

    myCli.run(cmdChar,sizeof(logInfo));
    myCli.show_error_detail();

    std::cout<<"main over"<<std::endl;
}


