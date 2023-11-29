#ifndef __CMDBASE_H__
#define __CMDBASE_H__

/*
    执行指令的基类，只可使用基指针，不允许创建
*/

#include "k_total_head.h"
#include "otlUse.h"

class CmdBase
{
public:
    CmdBase(){};
    virtual ~CmdBase(){};

    virtual int do_command()=0;

protected:
    COtlUse _cmdOtlUse;
};


#endif