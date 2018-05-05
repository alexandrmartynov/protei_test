#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include "InternetAddress.h"

class Socket
{
public:
    virtual ~Socket() {};
    virtual void send(const std::string& message) const = 0;
    virtual std::string receive() = 0;
    void setupAddress(int port);
    void bindSocket();
    void setNonBlockingSocket();
    void setSocket(int socket);
    int getSocket() const;
protected:
    int m_socket;
    InternetAddress m_addr;
};

#endif
