#ifndef SERVER_H
#define SERVER_H

#include "netinet/in.h"

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
    void send_tcp(char* message);
    void send_udp(char* message);
    char* receive_tcp();
    char* receive_udp();
    void calculate(char* message) const;

    sockaddr_in m_server_addr;
    sockaddr_in m_client_addr;
    int m_protocol;
    int m_socket;
    int m_client_socket;
};

#endif
