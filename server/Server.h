#ifndef SERVER_H
#define SERVER_H

#include "InternetAddress.h"
#include "Parser.h"
#include "Epoll.h"

class Server
{
public:
    explicit Server() = default;
    ~Server();
    int exec();
private:
    static const unsigned int MAX_EVENTS = 10;
    
    InternetAddress m_client_addr;

    Epoll m_epoll;
    Parser m_parser;
};

#endif
