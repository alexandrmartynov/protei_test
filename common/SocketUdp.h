#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include "Socket.h"

class SocketUdp: public Socket
{
public:
    explicit SocketUdp();
    virtual ~SocketUdp();
    virtual void send(const std::string& message) const override;
    virtual std::string receive() override;
private:   
    static const std::size_t BUFFER_SIZE = 65536;
};

#endif
