#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include "InternetAddress.h"

class Socket
{
public:
    virtual void createSocket() = 0;
    void setup(int port);
    void bindSocket();
    void setNonBlockingSocket();
    void setSocket(int socket);
    int getSocket() const;
    void closeSocket();
protected:
    std::string getMessage() const;

    int m_socket;
    InternetAddress m_addr;
    socklen_t m_addrlen;
};

#endif
