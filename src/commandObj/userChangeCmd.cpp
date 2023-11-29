#include "./include/userChangeCmd.h"

CUserChangeCmd::CUserChangeCmd()
    :_operatorUser(){}


int CUserChangeCmd::do_command()
{
    if(_cmdOtlUse.olt_init()==-1) 
    {
        std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;
        return -1;
    }

    int dealOperRe;
    if(_operType==CHANGE_USER)
    {
        dealOperRe=_cmdOtlUse.change_user(_operatorUser);
    }
    else if(_operType==ADD_USER)
    {
        dealOperRe=_cmdOtlUse.add_user(_operatorUser);
    }

    if(dealOperRe==-1) {std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;return -1;}
}

void CUserChangeCmd::set_operator_user(CUser &operatorUser)
{
    _operatorUser=operatorUser;
}
CUser CUserChangeCmd::get_operator_user()
{
    return _operatorUser;
}

void CUserChangeCmd::set_operator_type(OpratorType operType)
{
    _operType=operType;
}
CUserChangeCmd::~CUserChangeCmd()
{

}
