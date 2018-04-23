#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>

class Client
{
public:
    Client();
    int run();
    void setProtocolType(int protocol);
private:
    int setSocket();
    void writeServerAddress();
    void send_tcp(char* message);
    void send_udp(char* message);
    char* receive_tcp();
    char* receive_udp();
    char* getMessage() const;

    sockaddr_in m_server_addr;
    int m_socket;
    int m_client_socket;
    int m_protocol;
};

#endif
