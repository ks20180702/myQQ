#ifndef __DATAMSGCMD_H__
#define __DATAMSGCMD_H__


/*
    传输发送的消息的指令
*/

#include "dataCmdBase.h"
#include "msgData.h"

#include <sstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

class CDataMsgCmd:public CDataCmdBase
{
public:
    CDataMsgCmd(){};
    CDataMsgCmd(CMsgData msgData);

    virtual ~CDataMsgCmd() override{};

        //显示返回信息
    virtual void show_do_command_info(){};

        //执行当前指令
    virtual int do_command(COtlUse &cmdOtlUse)=0;

    //获取当前指令对象的json字符串
    virtual std::string get_command_obj_json()=0;

    //重新加载接收到的对象(服务器存储有用数据的对象)
    virtual void reload_recv_obj_by_json(cereal::JSONInputArchive &jsonIA)=0;

    //显示返回信息(用于查看服务器端执行情况)
    virtual void show_do_command_info()=0;



private:
    CMsgData _msgData;
};

#endif