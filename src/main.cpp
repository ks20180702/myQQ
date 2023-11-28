#include "add.h"
#include "sub.h"
#include <iostream>
#include "otlUse.h"

using namespace std;
#include "user.h"
int main()
{
	std::cout<<"main +++"<<std::endl;

    int selectReturn;
	COtlUse otlUse;
	CUser myUser,myUser02;
	CMsg myMsg;
	vector<CUser> friendLists;
	vector<CMsg> notRecvMsgs;
	otlUse.olt_init(); // otlUse.olt_init()==-1
	int msgNum=otlUse.get_not_recv_msg(1,notRecvMsgs);
	
	myMsg.set_msg_info(1,2,"2023-11-28 10:50:45","");
	otlUse.set_msg_send(myMsg);
	std::cout<<"error:"<<otlUse.get_errmsg()<<std::endl;

	return 0;// int selectReturn;
}


