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
    virtual int do_command(COtlUse &cmdOtlUse) override {
        std::cout<<"[Info]  this is null cmd"<<std::endl;
    };

    //获取当前指令对象的json字符串
    virtual std::string get_command_obj_json() override {
        return "";
    };
};

#endif