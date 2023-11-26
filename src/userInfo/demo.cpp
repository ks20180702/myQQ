//     char account[7]="123";
//     char password[16]="123";
//     char user_name[255]="ks";
// 	CUser user1(10,account,password,user_name,23);
// 	CUser user2(user1),user3;
// 	user1.set_user_info(99,account,password,user_name,23);
// 	std::cout<<user1.get_id()<<std::endl;
// 	std::cout<<user1.get_name()<<std::endl;

// 	std::cout<<user2.get_id()<<std::endl;
// 	std::cout<<user2.get_name()<<std::endl;

// 	if(user1==user2)
// 	{
// 		std::cout<<"user1 == user2"<<std::endl;
// 	}
// 	else{
// 		std::cout<<"user1 != user2"<<std::endl;
// 	}
// 	user3.print();
// 	user3=user1;
// 	user3.print();