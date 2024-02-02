#ifndef __USERCHANGECMD_H__
#define __USERCHANGECMD_H__

/*
    实现用户修改命令
    1.用户信息修改
    2.添加用户
*/
#include "cmdBase.h"
#include "user.h"
#include "msg.h"

class CUserChangeCmd:public CmdBase
{
public:
    enum OpratorType
    {
        CHANGE_USER, //修改用户
        ADD_USER //增加用户
    };
public:
    CUserChangeCmd();
    ~CUserChangeCmd() override;

    // 执行用户处理相关命令
    // 错误-1，0增加成功/修改成功
    virtual int do_command(COtlUse &cmdOtlUse) override;
    
    //获取当前指令对象的json字符串
    virtual std::string get_command_obj_json() override;

    // //获取当前指令执行完后将发送给(客户端/服务器端)的指令
    // //操作通知指令
    // virtual std::shared_ptr<CmdBase> get_send_command() override;

    //获取和设置用户(获取的并不是同一个)
    void set_operator_user(CUser &operatorUser);
    CUser get_operator_user();

    //设置该条指令的操作类型
    void set_operator_type(OpratorType operType);
private:
    //当前需要操作的对象
    CUser _operatorUser;

    OpratorType _operType;
};

#endif