#ifndef SERVER_H
#define SERVER_H

#include "netinet/in.h"
#include "../common/IOService.h"

#include <list>

class Server
{
public:
    Server();
    int run();
    void setProtocolType(unsigned short protocol);
private:
    void setSocket();
    void writeServerAddress();
    int connectClient();
    void calculate(const char* message) const;
    void displayList(std::list<int> &lst) const;

    sockaddr_in m_server_addr;
    sockaddr_in m_client_addr;
    unsigned short m_protocol;
    int m_socket;
    int m_client_socket;

    IOService m_service;
};

#endif
