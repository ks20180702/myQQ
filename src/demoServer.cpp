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
    std::cout<<"main +++"<<std::endl;

    CServerQQ mySer;

    mySer.connect_db();

    if(mySer.server_bind()==-1) mySer.show_error_detail();

    mySer.run();
}


