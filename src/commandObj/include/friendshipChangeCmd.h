#ifndef __FRIENDSHIPCHANGECMD_H__
#define __FRIENDSHIPCHANGECMD_H__

/*
    实现好友关系修改命令
    1.删除好友关系
    2.添加好友关系
*/
#include "cmdBase.h"
#include "user.h"
#include "msg.h"

class CFriendshipChangeCmd:public CmdBase
{
public:
    enum OperatorFriendShipType
    {
        DELETT_FRIEND, //删除好友关系
        ADD_FRIEND,//客户端的好友申请
        ADD_FRIEND_TO_CLIENT //增加好友关系申请指令(发送给客户端)
        ADD_FRIEND_TO_SERVER //好友同意(发送给服务器端)
    };
public:
    CFriendshipChangeCmd();
    CFriendshipChangeCmd(CUser &myUser,CUser &friendUser,OperatorFriendShipType friendType);
    ~CFriendshipChangeCmd() override;

    // 执行好友关系处理相关命令
    // 错误-1，0增加成功/修改成功
    virtual int do_command(COtlUse &cmdOtlUse) override;

    //下一条需执行的指令
    //空指令
    virtual std::shared_ptr<CmdBase> get_next_command() override ;

    //获取当前指令执行完后将发送给(客户端/服务器端)的指令
    // 发送的指令都是修改OperatorFriendShipType后的该条指令
    //OperatorFriendShipType
        // 1.DELETT_FRIEND 消息通知指令
        // 2.ADD_FRIEND 修改为(ADD_FRIEND_TO_CLIENT)并发送给对应好友客户端
        // 3.当前为客户端，同意：发送ADD_FRIEND_TO_SERVER
                     //   拒绝：空指令
    virtual std::shared_ptr<CmdBase> get_send_command() override;

    //设置需操作的用户，用户的好友，操作类型
    void set_user(CUser &myUser);
    void set_friend_user(CUser &friendUser);
    void set_operator_type(OperatorFriendShipType friendType);

private:
    //当前需要操作的对象
    CUser _myUser,_friendUser;

    OperatorFriendShipType _friendType;
};

#endif