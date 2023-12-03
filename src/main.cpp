#include <iostream>

using namespace std;
#include "user.h"
#include "otlUse.h"
#include "cmdBase.h"
#include "loginCmd.h"
#include "userChangeCmd.h"
#include "msgData.h"
#include "dataMsgCmd.h"

void test(CmdBase *base)
{
	base->do_command();
}
CDataBase* test2(CDataCmdBase *dataBase)
{
	dataBase->get_data_value();
}
int main()
{
	std::cout<<"main +++"<<std::endl;

	CMsgData msgData1(100);
	msgData1.print();
	CDataMsgCmd myDataMsg(msgData1);
	//传输CDataMsgCmd指令到客户端/服务器
	CMsgData *msgData=(CMsgData*)test2(&myDataMsg);
	msgData->print();
}


