#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include "Socket.h"
#include "InternetAddress.h"
#include <string>

class SocketUdp: public Socket
{
public:
    explicit SocketUdp();
    void dialog();
    std::string echo();
private:
    void send(const std::string& message) const;
    std::string receive();

    static const std::size_t BUFFER_SIZE = 65536;
};

#endif
