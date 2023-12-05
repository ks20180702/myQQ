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
    CmdBase()
        :_cmdOtlUse(),_next_command_ptr(nullptr),_send_command_ptr(nullptr){
            std::cout<<"[I] this is base class"<<std::endl;
        };

    virtual ~CmdBase(){};

    //执行当前指令
    virtual int do_command()=0;

    //下一条需执行的指令
    virtual std::shared_ptr<CmdBase> get_next_command()=0;

    //获取当前指令执行完后将发送给(客户端/服务器端)的指令
    virtual std::shared_ptr<CmdBase> get_send_command()=0;

protected:
    COtlUse _cmdOtlUse;

    // 下一条指令
    std::shared_ptr<CmdBase> _next_command_ptr;

    //将要发送的指令
    std::shared_ptr<CmdBase> _send_command_ptr;
};


#endif