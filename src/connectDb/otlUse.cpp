#include "./include/otlUse.h"

int COtlUse::olt_init()
{
    otl_connect::otl_initialize(); // initialize the database API environment
    try{
        _db.rlogon("DSN=pgsql;UID=postgres;PWD=123456;database=myQQ"); 
    }
    catch(otl_exception& p){ // intercept OTL exceptions
        strcpy(_errMsg,(char*)p.msg);
        return -1;
    }
    return 0;

}

int COtlUse::select_user_exist(CUser &myUser)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[128]={0};
        sprintf(sqlStr,"select pwd from user_info_table where account = '%s'",myUser.get_account());
        //std::cout<<sqlStr<<std::endl;
        otl_stream ostream(2, sqlStr,_db); 

        char pwd[16];
        while(!ostream.eof())
        { 
            ostream>>pwd;
            if(strcmp(pwd,myUser.get_password())!=0)
            {
                strcpy(_errMsg,"[E] the password is error");
                return 1; //用户密码错误
            }
            return 0; //用户登录成功   
        }
        strcpy(_errMsg,"[E] the user is not exist");
        return 2; //用户不存在
    }
    catch(otl_exception& p)
    {
        strcpy(_errMsg,(char*)p.msg);
        return -1;
    }
}

int COtlUse::get_user_by_id(int id,CUser &myUser)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[256]={0};
        sprintf(sqlStr,"select user_id,account,pwd,user_name,user_age,current_ip,cast(last_leave_time as varchar) \
                        from user_info_table where user_id = %d",id);
        //std::cout<<sqlStr<<std::endl;
        otl_stream ostream(2, sqlStr,_db); 

        int id;
        char act[7],pwd[16],name[255];
        int16_t age;
        char curIp[17],lastLeaveTime[32];
        while(!ostream.eof())
        { 
            ostream>>id>>act>>pwd>>name>>age>>curIp>>lastLeaveTime;
            myUser.set_user_info(id,act,pwd,name,age,curIp,lastLeaveTime);
            return 0; //用户登录成功   
        }
        strcpy(_errMsg,"[E]  id is not exist");
        return 2; //用户不存在
    }
    catch(otl_exception& p)
    {
        strcpy(_errMsg,(char*)p.msg);
        return -1;
    }
}

int COtlUse::set_user_id_by_account(CUser &myUser)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[128]={0};
        sprintf(sqlStr,"select user_id from user_info_table where account = '%s'",myUser.get_account());
        //std::cout<<sqlStr<<std::endl;
        otl_stream ostream(2, sqlStr,_db); 

        int id;
        while(!ostream.eof())
        { 
            ostream>>id;
            myUser.set_id(id);
            return id; //用户登录成功   
        }
    }
    catch(otl_exception& p)
    {
        strcpy(_errMsg,(char*)p.msg);
        return -1;
    }
}
int COtlUse::change_user(CUser &needChangeUser)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[256]={0};
        sprintf(sqlStr,
        "UPDATE user_info_table set pwd='%s',user_name='%s',user_age=%d ,\
        current_ip='%s' ,last_leave_time = '%s' where account='%s'",
        needChangeUser.get_password(),needChangeUser.get_name(),needChangeUser.get_age(),
        needChangeUser.get_ip(),needChangeUser.get_leave_time(),needChangeUser.get_account());
        std::cout<<sqlStr<<std::endl;
        otl_stream ostream(2, sqlStr,_db); 
        return 0;
    }
    catch(otl_exception& p)
    {
        strcpy(_errMsg,(char*)p.msg);
        return -1;
    } 
}
int COtlUse::add_user(CUser &addUser)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[256]="INSERT into user_info_table (account,pwd,user_name,user_age) \  
        values(:account<char[7]>,:pwd<char[16]>,:user_name<char[32]>,:user_age<int>)";

        //std::cout<<sqlStr<<std::endl;
        otl_stream ostream(2, sqlStr,_db); 
        ostream<<addUser.get_account()<<addUser.get_password()<<addUser.get_name()<<(int)addUser.get_age();
        ostream.flush();
        return 0;
    }
    catch(otl_exception& p)
    {
        strcpy(_errMsg,(char*)p.msg);
        return -1;
    }
}
int COtlUse::add_friend_info(CUser &myUser,CUser &myFriend)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[128]="INSERT into user_friend_info_table (user_id,user_friend_id) \
        VALUES (:user_id<int>,:user_friend_id<int>)";
        //std::cout<<sqlStr<<std::endl;
        otl_stream ostream(2, sqlStr,_db); 
        ostream<<myUser.get_id()<<myFriend.get_id();
        ostream.flush();
        return 0;
    }
    catch(otl_exception& p)
    {
        strcpy(_errMsg,(char*)p.msg);
        return -1;
    }
}
int COtlUse::get_user_friends(int id,vector<CUser> &friendLists)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[512]={0};
        char sqlFormat[512]="SELECT info.user_id,info.account,info.pwd, \
                            info.user_name,info.user_age,info.current_ip,cast (info.last_leave_time as varchar) \
                        FROM user_info_table as info \
                        INNER JOIN user_friend_info_table as friend \
                            on info.user_id=friend.user_friend_id and friend.user_id =%d";
        sprintf(sqlStr,sqlFormat,id);
        //std::cout<<sqlStr<<std::endl;
        otl_stream ostream(500, sqlStr,_db); 

        int id;
        char act[7],pwd[16],name[255];
        int16_t age;
        char curIp[17],lastLeaveTime[32];
        while(!ostream.eof())
        { 
            ostream>>id>>act>>pwd>>name>>age>>curIp>>lastLeaveTime;
            CUser friendUser(id,act,pwd,name,age,curIp,lastLeaveTime);
            friendLists.push_back(friendUser);
        }
        return friendLists.size();
    }
    catch(otl_exception& p)
    {
        strcpy(_errMsg,(char*)p.msg);
        friendLists.clear();
        return -1;
    }
}

int COtlUse::get_not_recv_msg(int recvId,vector<CMsg> &notRecvMsgs)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[512]={0};
        char sqlFormat[512]="SELECT msgInfo.msg_from_id,msgInfo.msg_to_id,cast(msgInfo.msg_datetime as varchar),\
                    cast(msgInfo.msg_content as varchar) from msg_info_table as msgInfo \
                INNER JOIN user_info_table as userInfo on msgInfo.msg_to_id=userInfo.user_id \
	                and (userInfo.last_leave_time < msgInfo.msg_datetime or userInfo.last_leave_time is NULL) \
                    and msgInfo.msg_to_id =%d";
        sprintf(sqlStr,sqlFormat,recvId);
        // std::cout<<sqlStr<<std::endl;
        otl_stream ostream(500, sqlStr,_db); 

        int sendId,recvId;
        char msgDateTime[32],content[512];
        while(!ostream.eof())
        {
            ostream>>sendId>>recvId>>msgDateTime>>content;
            CMsg tranMsg(sendId,recvId,msgDateTime,content);
            notRecvMsgs.push_back(tranMsg);
        }
        return notRecvMsgs.size();
    }
    catch(otl_exception& p)
    {
        strcpy(_errMsg,(char*)p.msg);
        notRecvMsgs.clear();
        return -1;
    }
}
int COtlUse::add_msg(CMsg &addMsg)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[256]="INSERT into msg_info_table (msg_from_id,msg_to_id,msg_datetime,msg_content) \  
        values(:msg_from_id<int>,:msg_to_id<int>,:msg_datetime<char[32]>,:msg_content<char[511]>)";

        std::cout<<sqlStr<<std::endl;
        otl_stream ostream(2, sqlStr,_db); 
        ostream<<addMsg.get_send_id()<<addMsg.get_recv_id()<<addMsg.get_msg_dt()<<addMsg.get_content();
        ostream.flush();
        return 0;
    }
    catch(otl_exception& p)
    {
        strcpy(_errMsg,(char*)p.msg);
        return -1;
    }
}
int COtlUse::set_msg_send(CMsg &sendMsg)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[128]={0};
        sprintf(sqlStr,
        "UPDATE msg_info_table set is_send='1' where msg_from_id= %d and msg_to_id=%d and msg_datetime='%s'",
        sendMsg.get_send_id(),sendMsg.get_recv_id(),sendMsg.get_msg_dt());
        std::cout<<sqlStr<<std::endl;
        otl_stream ostream(2, sqlStr,_db); 
        return 0;
    }
    catch(otl_exception& p)
    {
        strcpy(_errMsg,(char*)p.msg);
        return -1;
    } 
}
string COtlUse::get_errmsg()
{
    return _errMsg;
}

COtlUse::COtlUse()
{

}
COtlUse::~COtlUse()
{
    _db.logoff();
}

int COtlUse::_connect_on()
{
    if(_db.connected!=1)
    {
        memset(_errMsg,0,128);
        strcpy(_errMsg,"[E] connect is error");
        return -1;
    }
    else{
        return 0;
    }
}