#ifndef __CMDBASE_H__
#define __CMDBASE_H__

#include "k_total_head.h"
/*
    执行指令的基类，只可使用基指针，不允许创建
*/
class CmdBase
{
private:
    /* data */
public:
    CmdBase(){};
    virtual ~CmdBase()=0;

    virtual int do_command()=0;
};

CmdBase::~CmdBase(){}


#endif