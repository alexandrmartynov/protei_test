#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include "Socket.h"
#include <netinet/in.h>

class Socket_tcp: public Socket
{
public:
    Socket_tcp();
    virtual ~Socket_tcp();
    virtual void create() override;
    void send(const std::string& message) const;
    std::string receive();
    void disconnect();

    void binded(sockaddr_in& addr);
    void listening() const;
    void connected(sockaddr_in& addr, socklen_t addrlen) const;
    int accepted(sockaddr_in* addr, socklen_t* addrlen);
    void setSocket(int socket);
    int getSocket() const;
    void handle_message();
private:
    int m_socket;
};

#endif
