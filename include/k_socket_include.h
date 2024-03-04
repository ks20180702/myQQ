#ifndef __K_SOCKET_INCLUDE_H__
#define __K_SOCKET_INCLUDE_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 7070
#define SEND_RECV_BUF_SIZE 128//1024

#define MAX_EPOLL_SIZE 100

#endif