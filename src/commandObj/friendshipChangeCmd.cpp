#include "./include/friendshipChangeCmd.h"

CFriendshipChangeCmd::CFriendshipChangeCmd()
    :_myUser(),_friendUser(){}

CFriendshipChangeCmd::CFriendshipChangeCmd(CUser &myUser,CUser &friendUser,OperatorFriendShipType friendType)
{
    _myUser=myUser;
    _friendUser = friendUser;
    _friendType=friendType;
}
int CFriendshipChangeCmd::do_command(COtlUse &cmdOtlUse)
{
    if(_cmdOtlUse.olt_init()==-1) 
    {
        std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;
        return -1;
    }

    int dealOperRe=0;
    if(_friendType==DELETT_FRIEND)
    {
        dealOperRe=_cmdOtlUse.del_friendship(_myUser.get_id(),_friendType.get_id())
    }
    else if(_friendType==ADD_FRIEND_TO_SERVER)
    {
        //发送一条好友申请指令到好友客户端(本条指令)
    }
    if(dealOperRe==-1) {std::cout<<_cmdOtlUse.get_errmsg()<<std::endl;return -1;}
}

std::string CFriendshipChangeCmd::get_command_obj_json()
{
    return "";
}

void CFriendshipChangeCmd::set_user(CUser &myUser)
{
    _myUser=myUser;
}
void CFriendshipChangeCmd::set_friend_user(CUser &friendUser)
{
    _friendUser = friendUser;
}
void CFriendshipChangeCmd::set_operator_type(OperatorFriendShipType friendType)
{
    _friendType=friendType;
}
CFriendshipChangeCmd::~CFriendshipChangeCmd()
{

}
