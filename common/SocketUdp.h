#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include "Socket.h"

class SocketUdp: public Socket
{
public:
    explicit SocketUdp();
private:
    virtual void send(const std::string& message) override;
    virtual std::string receive() override;
};

#endif
