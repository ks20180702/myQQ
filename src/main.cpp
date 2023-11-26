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
	if(otlUse.olt_init()==-1)
	{
		std::cout<<"error:"<<otlUse.get_errmsg()<<std::endl;
		return -1;
	}
	else
	{
		std::cout<<"connect db is succeed"<<std::endl;
	}
	selectReturn=otlUse.select_user_exist("111111","123456",myUser);
	if(selectReturn==-1)
	{
		std::cout<<"error:"<<otlUse.get_errmsg()<<std::endl;
		return -1;
	}
	else if(selectReturn==0)
	{
		std::cout<<"[I]  login succeed"<<std::endl;
	}
	else if(selectReturn==1)
	{
		std::cout<<"[E]  the password is error"<<std::endl;
	}
	else{
		std::cout<<"[E]  user is not exist"<<std::endl;
	}
	return 0;
}
