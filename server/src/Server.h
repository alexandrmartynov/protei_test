#ifndef SERVER_H
#define SERVER_H

#include "netinet/in.h"

class Server
{
public:
    Server() = default;
    int run();
    void setProtocolType(int protocol);
private:
    void setSocket();
    void writeServerAddress();

    sockaddr_in m_server_addr;
    sockaddr_in m_client_addr;
    int m_protocol;
    int m_socket;
};

#endif
