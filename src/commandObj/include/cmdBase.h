#ifndef __CMDBASE_H__
#define __CMDBASE_H__

/*
    执行指令的基类，只可使用基指针，不允许创建
*/

// 【未实现】
// 暂时的几个指令的 get_next_command 和get_send_command都没实现
#include <memory>
#include "k_total_head.h"
#include "otlUse.h"

class CmdBase
{
public:
    CmdBase(){};

    virtual ~CmdBase(){};

    //执行当前指令
    virtual int do_command(COtlUse &cmdOtlUse)=0;

    //获取当前指令对象的json字符串
    virtual std::string get_command_obj_json()=0;

    enum CmdType
    {
        LOGIN_CMD,
        DTAT_MSG_CMD,
        USER_CHANGE_CMD,
        FRIEND_SHIP_CHANGE_CMD,
    };

public:
    CmdType _childCmdType;

    bool _childDoCommandReturn; //执行do_command的结果，成功true，失败false
};


#endif