#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include "Socket.h"
#include "InternetAddress.h"

class SocketUdp: public Socket
{
public:
    explicit SocketUdp() = default;
    ~SocketUdp();

    virtual void createSocket() override;

    void send(const std::string& message) const;
    std::string receive();
    void handle_message();
    std::string echo_message();
private:
    static const std::size_t BUFFER_SIZE = 65536;
};

#endif
