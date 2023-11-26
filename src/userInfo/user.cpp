#include "./user.h"


CUser::CUser()
    :_user_id(0),_account(""),_password(""),
    _user_name(""),_user_age(0){}

CUser::CUser(int user_id,char *account,char *password,char *user_name,int16_t user_age)
{
    _user_id=user_id;
    strcpy(_account,account);
    strcpy(_password,password);
    strcpy(_user_name,user_name);
    _user_age=user_age;
}
CUser::CUser(const CUser &other)
{
    *this=other;
}

void CUser::set_user_info(int user_id,char *account,char *password,
    char* user_name,int16_t user_age)
{
    _user_id=user_id;
    strcpy(_account,account);
    strcpy(_password,password);
    strcpy(_user_name,user_name);
    _user_age=user_age;
}
int CUser::get_id() const
{
    return _user_id;
}
char *CUser::get_account() const
{
    return (char*)_account;
}
char *CUser::get_password() const
{
    return (char*)_password;
}
char *CUser::get_name() const
{
    return (char*)_user_name;
}   
int16_t CUser::get_age() const
{
    return _user_age;
}
void CUser::print()
{
    std::cout<<"id = "<<this->get_id();
    std::cout<<" account = "<<this->get_account();
    std::cout<<" password = "<<this->get_password();
    std::cout<<" name = "<<this->get_name();
    std::cout<<" age = "<<this->get_age()<<std::endl;
}
bool CUser::operator==(const CUser &other)
{
    if(strcmp(_account,other.get_account())==0) return true;
    else return false;
}
CUser &CUser::operator=(const CUser &other)
{
    this->set_user_info(other.get_id(),other.get_account(),other.get_password(),
                        other.get_name(),other.get_age());

    return *this;
}

CUser::~CUser()
{
}