#include <iostream>

using namespace std;
#include "user.h"
#include "otlUse.h"
#include "cmdBase.h"
#include "loginCmd.h"

void test(CmdBase *base)
{
	base->do_command();
}
int main()
{
	std::cout<<"main +++"<<std::endl;

	CUser myUser(10,(char*)"123456",(char*)"123456",(char*)"ks",23,"","");
	CLoginCmd loginCmd;
	loginCmd.set_login_user(myUser);
	(loginCmd.get_login_user()).print();

    test(&loginCmd);

	return 0;// int selectReturn;
}


