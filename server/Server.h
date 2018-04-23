#ifndef SERVER_H
#define SERVER_H

#include "netinet/in.h"
#include "../common/IOService.h"

class Server
{
public:
    Server();
    int run();
    void setProtocolType(int protocol);
private:
    void setSocket();
    void writeServerAddress();
    int connectClient();
    void calculate(char* message) const;

    sockaddr_in m_server_addr;
    sockaddr_in m_client_addr;
    int m_protocol;
    int m_socket;
    int m_client_socket;

    IOService m_service;
};

#endif
