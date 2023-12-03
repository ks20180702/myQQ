#ifndef __NOTOPERATORCMD_H__
#define __NOTOPERATORCMD_H__

#include "cmdBase.h"

/*
    无操作指令，啥也不干，*****数据指令继承此指令
*/
class CNotOperatorCmd:public CmdBase
{
public:
    CNotOperatorCmd(){};
    ~CNotOperatorCmd() override{};

    //执行当前指令
    virtual int do_command() override {
        std::cout<<"[Info]  this is null cmd"<<std::endl;
    };

    //下一条需执行的指令
    virtual std::shared_ptr<CmdBase> get_next_command() override {_next_command_ptr=nullptr;return _next_command_ptr;};

    //获取当前指令执行完后将发送给(客户端/服务器端)的指令
    virtual std::shared_ptr<CmdBase> get_send_command() override {_send_command_ptr=nullptr;return _send_command_ptr;};
};

#endif