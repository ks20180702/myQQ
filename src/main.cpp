#include "add.h"
#include "sub.h"
#include <iostream>
#include "otlUse.h"

#include "user.h"
int main()
{
	std::cout<<"main +++"<<std::endl;

    int selectReturn;
	COtlUse otlUse;
	CUser myUser;
	std::vector<CUser> friendLists;
	if(otlUse.olt_init()==-1)
	{
		std::cout<<"error:"<<otlUse.get_errmsg()<<std::endl;
		return -1;
	}
	else
	{
		std::cout<<"connect db is succeed"<<std::endl;
	}
	int friendNum=otlUse.get_user_friends(1,friendLists);
	std::cout<<"friend num = "<<friendNum<<std::endl;
	return 0;
}
