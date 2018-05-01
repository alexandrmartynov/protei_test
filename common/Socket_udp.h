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

    void send(const std::string& message, InternetAddress& addr) const;
    std::string receive(InternetAddress& addr);
    void handle_message(InternetAddress& addr);
    std::string echo_message(InternetAddress& addr);
private:
    static const std::size_t BUFFER_SIZE = 65536;
};

#endif
