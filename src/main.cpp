#include <iostream>

using namespace std;
#include "user.h"
#include "otlUse.h"
#include "cmdBase.h"
#include "loginCmd.h"
#include "userChangeCmd.h"
void test(CmdBase *base)
{
	base->do_command();
}
int main()
{
	std::cout<<"main +++"<<std::endl;

	CUser myUser(10,(char*)"123456",(char*)"123456789",(char*)"ks",23,"","");
	// CLoginCmd loginCmd;
	// loginCmd.set_login_user(myUser);
	// (loginCmd.get_login_user()).print();

	CUserChangeCmd userChangeCmd;
	userChangeCmd.set_operator_type(CUserChangeCmd::CHANGE_USER);
	userChangeCmd.set_operator_user(myUser);
    test(&userChangeCmd);

	return 0;// int selectReturn;
}


