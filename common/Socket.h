#ifndef SOCKET_H
#define SOCKET_H

#include "InternetAddress.h"

class Socket
{
public:
    Socket() = default;
    ~Socket();
    Socket(const Socket& obj) = delete;
    Socket& operator=(const Socket& obj) = delete;
    void setup(int port);
    void bindSocket();
    void setNonBlockingSocket();
    void setSocket(int socket);
    int getSocket() const;
    void closeSocket() const;
protected:
    int m_socket;
    InternetAddress m_addr;
    socklen_t m_addrlen;
};

#endif
