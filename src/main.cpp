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

	CLoginCmd loginCmd;
    test(&loginCmd);

	return 0;// int selectReturn;
}


