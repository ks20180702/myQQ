#include "./include/loginCmd.h"
#include "./include/notOperatorCmd.h"

CLoginCmd::CLoginCmd()
    :_loginUser(){}
CLoginCmd::CLoginCmd(CUser &loginUser)
{
    _loginUser=loginUser;
}  
int CLoginCmd::do_command()
{
    if(_cmdOtlUse.olt_init()==-1) 
    {
        std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;
        return -1;
    }
    
    int existRe,friendNumRe,notRevcMsgNumRe;

    // 1.检查用户账号密码，
    existRe=_cmdOtlUse.select_user_exist(_loginUser);
    if(existRe!=0) {std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;return -1;}

    // 使用户的id与account保持一致(测试时自己手动创建用户，可能会不一致)，正式不会这样
    if(_cmdOtlUse.set_user_id_by_account(_loginUser)==-1) 
    {std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;return -1;}

    // 2.获取该用户的好友数据
    friendNumRe=_cmdOtlUse.get_user_friends(_loginUser.get_id(),_friendLists);
    if(friendNumRe==-1) {std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;return -1;}
    std::cout<<"[I]  user have friend num = "<<friendNumRe<<std::endl;

    // 3.获取消息未接收情况
    notRevcMsgNumRe=_cmdOtlUse.get_not_recv_msg(_loginUser.get_id(),_notRecvMsgsLists);
    if(notRevcMsgNumRe==-1) {std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;return -1;}
    std::cout<<"[I]  user have no recv msg num = "<<notRevcMsgNumRe<<std::endl;
    
    // 4.发送用户成功消息+好友信息+为接收的信息数量(不加内容)
    return 0;
}
std::shared_ptr<CmdBase> CLoginCmd::get_next_command()
{
    if(_next_command_ptr == nullptr)
    {
        _next_command_ptr=std::make_shared<CNotOperatorCmd>();
    }
    
    return _next_command_ptr;
}

std::shared_ptr<CmdBase> CLoginCmd::get_send_command()
{
    if(_send_command_ptr == nullptr)
    {
        _send_command_ptr=std::make_shared<CNotOperatorCmd>();
    }
    
    return _send_command_ptr;
}

void CLoginCmd::set_login_user(CUser &loginUser)
{
    _loginUser=loginUser;
}
CUser CLoginCmd::get_login_user()
{
    return _loginUser;
}

std::vector<CUser> &CLoginCmd::get_friend_lists()
{
    return _friendLists;
}
std::vector<CMsg> &CLoginCmd::get_not_recv_msg_lists()
{
    return _notRecvMsgsLists;
}
void CLoginCmd::set_friend_lists(std::vector<CUser> &friendLists)
{
    _friendLists=friendLists;
}


// int CLoginCmd::get_obj_sizeof()
// {
//     return sizeof(*this)+_friendLists.size()*sizeof(CUser)+_notRecvMsgsLists.size()*sizeof(CMsg);
// }

CLoginCmd::~CLoginCmd(){
    _friendLists.clear();
}

