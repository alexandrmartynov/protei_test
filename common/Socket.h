#ifndef SOCKET_H
#define SOCKET_H

#include<string>
#include "InternetAddress.h"

class Socket
{
public:
    virtual void create() = 0;
    void setup(int port);
    void bindSocket();
    void setSocket(int socket);
    int getSocket() const;
    void closeSocket();
protected:
    std::string getMessage();

    int m_socket;
    InternetAddress m_addr;
    socklen_t m_addrlen;

    std::string m_message;
};

#endif
