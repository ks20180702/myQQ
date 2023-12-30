#ifndef __SERVERQQ_H__
#define __SERVERQQ_H__

#include "k_socket_include.h"
#include "k_total_head.h"
class CServerQQ
{
public:
    CServerQQ();
    ~CServerQQ();

    int server_bind();

    //运行程序，启动服务端
    //错误-1，
    int run();

    
    char *get_error();

    //显示上一个错误的错误详情
    void show_error_detail();

    int& get_socket();

private:
    char _errMsg[128]="OK";

    int _serSoc;
};


#endif
