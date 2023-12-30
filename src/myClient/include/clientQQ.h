#ifndef __CLIENTQQ_H__
#define __CLIENTQQ_H__

#include "k_socket_include.h"
#include "k_total_head.h"
class ClientQQ
{
public:
    ClientQQ();
    ~ClientQQ();

    //获取socket，连接指定的ip
    //错误-1，成功0
    int client_init(char *ipAddr);

    //初始化select相关设置
    //错误-1，0成功
    int select_init();

    //运行程序，启动客户端
    //错误-1，成功则循环等待
    int run(char *testStr,int n);

    //分批发送数据，暂定1024
    //错误-1，成功0
    int send_part(char *sendStr,int n);
    
    char *get_error();

    //显示上一个错误的错误详情
    void show_error_detail();

    int& get_socket();

private:
    char _errMsg[128]="OK";

    int _cliSoc;
    struct sockaddr_in _serAddr;

    fd_set _globalFdset;
};


#endif