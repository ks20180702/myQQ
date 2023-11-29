#ifndef __LOGINCMD_H
#define __LOGINCMD_H

#include "cmdBase.h"

class CLoginCmd:public CmdBase
{

public:
    CLoginCmd(){};
    ~CLoginCmd();

    virtual int do_command() override;

private:
    /* data */
};

#endif