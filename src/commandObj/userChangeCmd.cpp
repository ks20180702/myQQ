#include "./include/userChangeCmd.h"
#include "./include/notOperatorCmd.h"

CUserChangeCmd::CUserChangeCmd()
    :_operatorUser(){}


int CUserChangeCmd::do_command(COtlUse &cmdOtlUse)
{
    // if(_cmdOtlUse.olt_init()==-1) 
    // {
    //     std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;
    //     return -1;
    // }

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

std::shared_ptr<CmdBase> CUserChangeCmd::get_next_command()
{
    if(_next_command_ptr == nullptr)
    {
        _next_command_ptr=std::make_shared<CNotOperatorCmd>();
    }
    
    return _next_command_ptr;
}

std::shared_ptr<CmdBase> CUserChangeCmd::get_send_command()
{
    if(_send_command_ptr == nullptr)
    {
        _send_command_ptr=std::make_shared<CNotOperatorCmd>();
    }
    
    return _send_command_ptr;
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
