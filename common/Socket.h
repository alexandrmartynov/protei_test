#ifndef SOCKET_H
#define SOCKET_H

#include "InternetAddress.h"
#include <string>

class Socket
{
public:
    void dialog();
    std::string echo();
    void setupAddress(int port);
    void bindSocket();
    void setNonBlockingSocket();
    void setSocket(int socket);
    int getSocket() const;
protected:
    virtual void send(const std::string& message) = 0;
    virtual std::string receive() = 0;

    static const std::size_t BUFFER_SIZE = 65536;

    int m_socket;
    InternetAddress m_addr;
    socklen_t m_addrlen;
};

#endif
