#ifndef __OTLUSE_H__
#define __OTLUSE_H__

#define OTL_ODBC // CompileOTL 4.0/ODBC  
#define OTL_ODBC_UNIX // uncomment this line if UnixODBC is used
//#define OTL_UNICODE // Compile OTL with Unicode    --->屏蔽 使用时中文显示乱码

#include <k_total_head.h>
#include "otlv4.h" // include the OTL 4.0 header file
#include "user.h"

#include <vector>

using namespace std;

class COtlUse
{
public:
    COtlUse();
    ~COtlUse();

    int olt_init();

    // 判断用户是否存在
    // 错误-1，存在1，不存在2
    int select_user_exist(string account,string password,CUser &myUser);

    // 获取用户的好友列表
    // return 好友数量，错误返回-1
    int get_user_friends(int id,vector<CUser> &friendLists);

    // 获取上一次操作的错误消息
    string get_errmsg();

private:
    otl_connect _db; // connect object
    char _errMsg[128]="0";

    // 判断连接是否成功
    int _connect_on();
};

#endif