#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include "../common/IOService.h"

class Client
{
public:
    Client();
    int run();
    void setProtocolType(int protocol);
private:
    int setSocket();
    void writeServerAddress();
    char* getMessage() const;

    sockaddr_in m_server_addr;
    int m_socket;
    int m_client_socket;
    int m_protocol;

    IOService m_service;
};

#endif
