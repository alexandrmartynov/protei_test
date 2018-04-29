#ifndef SERVER_H
#define SERVER_H

#include "InternetAddress.h"
#include "Parser.h"

class Server
{
public:
    explicit Server() = default;
    ~Server();
    int exec();
private:
    static const unsigned int MAX_EVENTS = 10;
    
    InternetAddress m_client_addr;
    InternetAddress m_server_addr;

    Parser m_parser;
};

#endif
