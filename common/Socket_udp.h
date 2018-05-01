#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include "Socket.h"
#include "InternetAddress.h"

class Socket_udp: public Socket
{
public:
    explicit Socket_udp() = default;
    Socket_udp(const Socket_udp& temp);
    ~Socket_udp() {};

    virtual void create() override;

    void send(const std::string& message);
    std::string receive();
    void handle_message();
    std::string echo_message();
private:
    static const std::size_t BUFFER_SIZE = 65536;
};

#endif
