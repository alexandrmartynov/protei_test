#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include "Socket.h"
#include <netinet/in.h>

class SocketUDP: public Socket
{
public:
    SocketUDP();
    virtual ~SocketUDP();
    virtual int create() override;
    virtual void send(const std::string& message) const override;
    virtual std::string receive() override; 
private:
    int m_socket;
    sockaddr_in m_addr;
};

#endif
