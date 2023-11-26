#include "./include/otlUse.h"

void COtlUse::select()
{ 
    try{
        otl_stream ostream1(500, // buffer size
            "select * from user_info_table;",// SELECT statement
            _db // connect object
            ); 

        int user_id;
        char account[6];
        char password[16];
        char user_name[255];
        int16_t user_age;

        while(!ostream1.eof())
        { // while not end-of-data
            //ostream1>>id>>user>>name;
            ostream1>>user_id;
            ostream1>>account;
            ostream1>>password;
            ostream1>>user_name;
            ostream1>>user_age;
            cout<<"id="<<user_id<<endl;
            cout<<"age="<<user_age<<endl;
            cout<<"name="<<user_name<<endl;
        }
    }
    catch(otl_exception& p)
    { // intercept OTL exceptions
        cout<<"otl_exception:"<<endl;
        cerr<<p.msg<<endl; // print out error message
        cerr<<p.stm_text<<endl; // print out SQL that caused the error
        cerr<<p.var_info<<endl; // print out the variable that caused the error
    }
}

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
        char act[7];
        char pwd[16];
        char name[255];
        int16_t age;
        while(!ostream.eof())
        { 
            ostream>>id;
            ostream>>act;
            ostream>>pwd;
            ostream>>name;
            ostream>>age;
            if(strcmp(pwd,password.c_str())!=0)
            {
                return 1; //用户密码错误
            }
            myUser.set_user_info(id,act,pwd,name,age);
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