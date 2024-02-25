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

    char testStr[1024]={0};
    FILE *fTest;
    fTest=fopen("/home/ks02/K_runDir/myQQ/src/kk.txt", "rb");

    int r=fread(testStr, sizeof(char), 1024, fTest);
    fclose(fTest);

    myCli.run(testStr,1024);

    myCli.show_error_detail();

    std::cout<<"main over"<<std::endl;
}


