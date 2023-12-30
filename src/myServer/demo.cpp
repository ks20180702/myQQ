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
	
int main()
{
    std::cout<<"main +++"<<std::endl;

    CServerQQ mySer;
    if(mySer.server_bind()==-1) mySer.show_error_detail();

    int listenSoc=mySer.get_socket();
    struct sockaddr_in serAddr,cliAddr;
    socklen_t serLen,cliLen;
    int lr,bindRe;  //记录一些函数的返回值，-1时表示出错
    size_t r,w;
    char buf[128]={0};
    while(1)
    {
        r=recvfrom(listenSoc,buf,128,0,
                (struct sockaddr*)&cliAddr,&cliLen);
        printf("%s",buf);

        strcpy(buf,"hello this is server\n");
        w=sendto(listenSoc,buf,128,0,
                (struct sockaddr*)&cliAddr,cliLen);
    }

}


