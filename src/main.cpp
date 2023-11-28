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
	CUser myUser;
	vector<CUser> friendLists;
	otlUse.olt_init(); // otlUse.olt_init()==-1

	int friendNum=otlUse.get_user_friends(1,friendLists);
	if(friendNum==-1)
	{
		std::cout<<"error:"<<otlUse.get_errmsg()<<std::endl;
		return -1;
	}
	for(vector<CUser>::iterator it=friendLists.begin();it!=friendLists.end();it++)
	{
		it->print();
	}
	return 0;// int selectReturn;
}


