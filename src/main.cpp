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
	CUser myUser((char*)"112222",(char*)"123456",(char*)"ks",23);
	CMsg myMsg;
	vector<CUser> friendLists;
	vector<CMsg> notRecvMsgs;
	otlUse.olt_init(); // otlUse.olt_init()==-1
	int msgNum=otlUse.get_not_recv_msg(1,notRecvMsgs);

	myMsg.set_msg_info(1,2,(char*)"2023-11-28 10:50:45",(char*)"你好");

	CUser myUser02((char*)"113333",(char*)"123456",(char*)"ks_friend",23);
	otlUse.add_user(myUser02);
	std::cout<<"myUser02 id = "<<otlUse.set_user_id_by_account(myUser02)<<std::endl;
	
	otlUse.add_user(myUser);
	std::cout<<"myUser id = "<<otlUse.set_user_id_by_account(myUser)<<std::endl;

	otlUse.add_friend_info(myUser,myUser02);
	otlUse.add_friend_info(myUser02,myUser);
	std::cout<<"error:"<<otlUse.get_errmsg()<<std::endl;

	return 0;// int selectReturn;
}


