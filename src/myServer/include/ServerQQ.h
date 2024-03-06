#ifndef __SERVERQQ_H__
#define __SERVERQQ_H__

#include "k_socket_include.h"
#include "k_total_head.h"

#include "loginCmd.h"
#include "otlUse.h"
#include "userChangeCmd.h"
#include "friendshipChangeCmd.h"
#include "cmdCreateFactory.h"

#include <vector>
#include <memory>
#include <map>
class CServerQQ
{
public:
    CServerQQ();
    ~CServerQQ();

    int server_bind();

    int connect_db(char *connectStr=nullptr);

    // 由于pthread_create不能用非静态成员函数作为回调，所以套一层，该函数用于调用实际执行的成员函数
    static void* pthread_fun(void *arg);
    // 线程中实际执行的函数
    void pthread_recv_and_send_msg();


    //运行程序，启动服务端
    //错误-1，
    int run();

    //接收字节流
    //成功0，失败-1
    int recv_cmd_part(char *buf,int readNum);

    //解析接收的字节流，转成对应对象
    //成功0，异常-1(暂时未增加)
    // cmdStr 客户端发送来的字符串
    // returnCmdJosnStr 服务器端处理完毕后的字符串
    int param_cmd_str(std::string cmdStr,std::string &returnCmdJosnStr);

    //发送字节流
    //成功0，异常-1
    int send_part(char *sendStr,int n,sockaddr_in &cliAddr);

    char *get_error();

    //显示上一个错误的错误详情
    void show_error_detail();

    int& get_socket();

    //测试方法
    void Test();

private:
    char _errMsg[128]="OK";

    int _serSoc;

    //数据库操作对象
    COtlUse _cmdOtlUse;

    std::map<string,string> _clientCmdStrMap;

    std::shared_ptr<CmdCreateFactory> _factoryCreater;
};


#endif
