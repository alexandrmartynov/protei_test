#ifndef SERVER_H
#define SERVER_H

#include "InternetAddress.h"
#include "Parser.h"
#include "Epoll.h"

class Server
{
public:
    explicit Server();
    ~Server();
    int exec();
private:
    static const unsigned int MAX_EVENTS = 10;
    static const unsigned int PORT = 8080;

    Epoll* m_epoll;
    Parser m_parser;
    InternetAddress m_client_addr;
};

#endif
