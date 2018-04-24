#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include "../common/IOService.h"

class Client
{
public:
    explicit Client();
    int exec();
private:
    void writeServerAddress();

    sockaddr_in m_server_addr;
    int m_socket;
    int m_client_socket;
    unsigned short m_protocol;

    IOService m_service;
};

#endif
