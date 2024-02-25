#ifndef __NULLCMD_H__
#define __NULLCMD_H__

#include "cmdBase.h"

/*
    空指令，啥也不干
*/
class CNullCmd:public CmdBase
{
public:
    CNullCmd(){};
    ~CNullCmd() override{};

    //执行当前指令
    virtual int do_command(COtlUse &cmdOtlUse) override {
        std::cout<<"[Info]  this is null cmd"<<std::endl;
    };

    //下一条需执行的指令
    virtual std::shared_ptr<CmdBase> get_next_command() override {_next_command_ptr=nullptr;return _next_command_ptr;};

    //获取当前指令执行完后将发送给(客户端/服务器端)的指令
    virtual std::shared_ptr<CmdBase> get_send_command() override {_send_command_ptr=nullptr;return _send_command_ptr;};
};

#endif