#ifndef __DATACCMDBASE_H__
#define __DATACCMDBASE_H__

#include "notOperatorCmd.h"
#include "dataBase.h"
/*
    数据指令，继承CNotOperatorCmd，作为携带数据的指令
*/
class CDataCmdBase:public CNotOperatorCmd
{
public:
    CDataCmdBase(){};
    virtual ~CDataCmdBase(){};

    virtual CDataBase* get_data_value()=0;
};

#endif