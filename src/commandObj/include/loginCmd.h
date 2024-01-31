#ifndef __LOGINCMD_H
#define __LOGINCMD_H

/*
    实现登录命令
     默认_loginUser是设置好的，id与account保持一致
*/
#include "cmdBase.h"
#include "user.h"
#include "msg.h"

#include <sstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

class CLoginCmd:public CmdBase
{

public:
    CLoginCmd();
    CLoginCmd(CUser &loginUser);
    ~CLoginCmd();

    // 收到该指令时，执行预设功能
    //  1.检查用户账号密码，2.获取该用户的好友数据，3.获取消息未接收情况
    // 错误-1，成功0
    virtual int do_command(COtlUse &cmdOtlUse) override;

    //下一条需执行的指令
    //空指令
    virtual std::shared_ptr<CmdBase> get_next_command() override ;

    //获取当前指令执行完后将发送给(客户端/服务器端)的指令
    //操作通知指令
    virtual std::shared_ptr<CmdBase> get_send_command() override;

    // 设置当前的登录对象
    void set_login_user(CUser &loginUser);

    // 获取登录对象的值(非同一个)，用于查看
    CUser get_login_user();

    //返回好友列表的引用
    std::vector<CUser> &get_friend_lists();

    //返回未接收消息的引用
    vector<CMsg> &get_not_recv_msg_lists();

    //设置好友列表
    void set_friend_lists(std::vector<CUser> &friendLists);

    //设置未接收消息
    void set_not_recv_msg_lists(std::vector<CMsg> &notRecvMsgsLists);

    //序列化
    template <class Archive>
    void serialize(Archive & ar)
	{
		ar(cereal::make_nvp("_childDoCommandReturn", _childDoCommandReturn),
        cereal::make_nvp("_loginUser", _loginUser),
        cereal::make_nvp("_friendLists", _friendLists), 
        cereal::make_nvp("_notRecvMsgsLists", _notRecvMsgsLists));
	}

private:
    //当前用于登录的对象
    CUser _loginUser;

//可以考虑不作为成员变量
    //当前用户的好友信息
    std::vector<CUser> _friendLists;

    //用户为接收的信息
    std::vector<CMsg> _notRecvMsgsLists;
};

#endif