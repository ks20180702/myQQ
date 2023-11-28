#ifndef __OTLUSE_H__
#define __OTLUSE_H__

#define OTL_ODBC // CompileOTL 4.0/ODBC  
#define OTL_ODBC_UNIX // uncomment this line if UnixODBC is used
//#define OTL_UNICODE // Compile OTL with Unicode    --->屏蔽 使用时中文显示乱码

#include <k_total_head.h>
#include "otlv4.h" // include the OTL 4.0 header file
#include "user.h"
#include "msg.h"

#include <vector>

using namespace std;

/*
    类型检查全都放到调用者中，使用db操作类默认数据都是正确
*/


class COtlUse
{
public:
    COtlUse();
    ~COtlUse();

    int olt_init();

    // 判断用户是否存在
    // 错误-1，存在1，不存在2
    int select_user_exist(string account,string password,CUser &myUser);

    // 获取用户信息
    // 错误返回-1 ，成功0
    int get_user_by_id(int id,CUser &myUser);

    // 通过账号来获取数据库中的自增id，将id设置到user中
    // 错误返回-1，成功返回id
    int set_user_id_by_account(CUser &myUser);

    // 获取用户的好友列表
    // return 好友数量，错误返回-1
    int get_user_friends(int id,vector<CUser> &friendLists);

    // 将该账号的用户修改为输入的值，用户离开后会修改ip为""，并更新离开时间
    // 错误-1，0成功
    int change_user(CUser &needChangeUser);

    // 增加用户
    // 错误-1，0成功
    int add_user(CUser &addUser);

    // 增加用户好友
    // 错误-1，0成功
    int add_friend_info(CUser &myUser,CUser &myFriend);

    //获取离线后未接收的消息
    // return 未接收到的消息条数，错误返回-1
    int get_not_recv_msg(int recvId,vector<CMsg> &notRecvMsgs);
    
    // 设置当前的消息为已发送
    // 错误-1，0成功
    int set_msg_send(CMsg &sendMsg);


    // 获取上一次操作的错误消息
    string get_errmsg();

private:
    otl_connect _db; // connect object
    char _errMsg[128]="OK";

    // 判断连接是否成功
    int _connect_on();
};

#endif