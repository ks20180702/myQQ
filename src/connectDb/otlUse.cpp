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


int COtlUse::select_user_exist(string account,string password,CUser &myUser)
{
    if(_connect_on()==-1) return -1;
    try{
        char sqlStr[128]={0};
        sprintf(sqlStr,"select * from user_info_table where account = '%s'",account.c_str());
        //std::cout<<sqlStr<<std::endl;
        otl_stream ostream(500, sqlStr,_db); 

        int id;
        char act[7],pwd[16],name[255];
        int16_t age;
        char curIp[17],lastLeaveTime[32];
        while(!ostream.eof())
        { 
            ostream>>id>>act>>pwd>>name>>age>>curIp>>lastLeaveTime;
            if(strcmp(pwd,password.c_str())!=0)
            {
                return 1; //用户密码错误
            }
            myUser.set_user_info(id,act,pwd,name,age,curIp,lastLeaveTime);
            return 0; //用户登录成功
            
        }
        return 2; //用户不存在
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
        std::cout<<sqlStr<<std::endl;
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