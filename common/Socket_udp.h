#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include "Socket.h"
#include <netinet/in.h>

class Socket_udp: public Socket
{
public:
    Socket_udp();
    virtual ~Socket_udp();
    virtual void create() override;
    void binded(sockaddr_in& addr, const socklen_t& addrlen) const;
    void send(const std::string& message, sockaddr_in& addr) const;
    void disconnect();
    std::string receive(sockaddr_in& addr, socklen_t& addlen);
    int getSocket() const;
private:
    int m_socket;
};

#endif
